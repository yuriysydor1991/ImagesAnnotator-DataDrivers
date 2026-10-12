#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h"
#include "src/dbs/AnnotationsDBs/LibraryImpl.h"
#include "src/records/RecordsFactory.h"
#include "src/zlib/ZlibController.h"

using namespace testing;
using iadd0zlibi::ZlibController;
using iannotator::dbs::annotations::Imgajz1LibraryImpl;
using iannotator::dbs::annotations::LibraryImpl;
using ImagesAnnotatorDataDrivers013::ImageRecord;
using ImagesAnnotatorDataDrivers013::ImageRecordRect;
using ImagesAnnotatorDataDrivers013::ImageRecordsSet;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace fs = std::filesystem;

namespace
{

/**
 * @brief Drives the imgajz1 project file driver over real files in a temporary
 * directory, since reading and writing them is the whole of its job.
 */
class UTEST_Imgajz1LibraryImpl : public Test
{
 public:
  void SetUp() override
  {
    root = fs::path{TempDir()} / "utest_imgajz1_library_impl";
    imagesDir = root / "images";

    fs::remove_all(root);
    fs::create_directories(imagesDir);

    project = root / "project.imgajz1";
    json = root / "project.json";
  }

  void TearDown() override { fs::remove_all(root); }

  ImageRecordsSet one_record() const
  {
    auto ir = std::make_shared<ImageRecord>("a.png", imagesDir.string());

    ir->iwidth = 640;
    ir->iheight = 480;
    ir->imageScale = 1.5;
    ir->rects.emplace_back(
        std::make_shared<ImageRecordRect>("dog", 50, 20, 100, 40));

    return ImageRecordsSet{ir};
  }

  /// @brief The decompressed text of the imgajz1 file.
  std::string decompressed() const
  {
    const ZlibController::buffer text =
        ZlibController::create()->uncompress_from_file(project.string());

    return std::string{text.cbegin(), text.cend()};
  }

  /// @brief Stores the text into the imgajz1 file gzip compressed.
  void write_compressed(const std::string& text) const
  {
    ASSERT_TRUE(ZlibController::create()->compress_to_file(
        project.string(), {text.cbegin(), text.cend()}));
  }

  void write_file(const std::string& content) const
  {
    std::ofstream f(project, std::ios::binary);
    f << content;
  }

  static std::string read_file(const fs::path& fpath)
  {
    std::ifstream f(fpath, std::ios::binary);
    return std::string{std::istreambuf_iterator<char>(f),
                       std::istreambuf_iterator<char>()};
  }

  /// @brief The text of the JSON project file of one_record().
  std::string json_project() const
  {
    EXPECT_TRUE(LibraryImpl::create()->store(json.string(), one_record()));

    return read_file(json);
  }

  bool loads(ImageRecordsSet& loaded) const
  {
    return Imgajz1LibraryImpl::create()->load(project.string(), loaded);
  }

  fs::path root;
  fs::path imagesDir;
  fs::path project;
  fs::path json;
};

}  // namespace

TEST_F(UTEST_Imgajz1LibraryImpl, stores_and_loads_the_records_back)
{
  auto impl = Imgajz1LibraryImpl::create();

  ASSERT_NE(impl, nullptr);
  ASSERT_TRUE(impl->store(project.string(), one_record()));
  ASSERT_TRUE(fs::is_regular_file(project));

  ImageRecordsSet loaded;

  ASSERT_TRUE(impl->load(project.string(), loaded));
  ASSERT_EQ(loaded.size(), 1U);
  ASSERT_NE(loaded.front(), nullptr);

  EXPECT_TRUE(ImageRecord::equal(loaded, one_record()));
  EXPECT_EQ(loaded.front()->get_full_path(), (imagesDir / "a.png").string());
  ASSERT_EQ(loaded.front()->rects.size(), 1U);
  EXPECT_EQ(loaded.front()->rects.front()->name, "dog");
}

TEST_F(UTEST_Imgajz1LibraryImpl, writes_the_gzip_compressed_json_project_file)
{
  ASSERT_TRUE(
      Imgajz1LibraryImpl::create()->store(project.string(), one_record()));

  EXPECT_TRUE(ZlibController::is_gzip_file(project.string()));

  // the very text of the JSON project file of the same project
  EXPECT_EQ(decompressed(), json_project());
  EXPECT_LT(fs::file_size(project), fs::file_size(json));
}

TEST_F(UTEST_Imgajz1LibraryImpl, stores_and_loads_an_empty_project)
{
  auto impl = Imgajz1LibraryImpl::create();

  ASSERT_TRUE(impl->store(project.string(), ImageRecordsSet{}));

  const auto document = nlohmann::json::parse(decompressed());

  EXPECT_TRUE(document[S::annotations_field_name].empty());

  ImageRecordsSet loaded{one_record()};

  ASSERT_TRUE(impl->load(project.string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, stores_the_application_identity_it_was_given)
{
  auto impl = Imgajz1LibraryImpl::create();

  impl->set_app_info("MyAnnotator", "1.2.3");

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  const auto document = nlohmann::json::parse(decompressed());

  EXPECT_EQ(document["MyAnnotator"][S::file_version_field_name], "1.2.3");

  ImageRecordsSet loaded;

  EXPECT_TRUE(impl->load(project.string(), loaded));
  EXPECT_EQ(loaded.size(), 1U);

  auto other = Imgajz1LibraryImpl::create();

  other->set_app_info("SomebodyElse", "1.2.3");

  ImageRecordsSet refused;

  EXPECT_FALSE(other->load(project.string(), refused));
  EXPECT_TRUE(refused.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, takes_the_settings_of_the_given_driver)
{
  auto jsonImpl = LibraryImpl::create();
  auto factory = iannotator::records::RecordsFactory::create();

  jsonImpl->set_app_info("MyAnnotator", "1.2.3");
  jsonImpl->set_records_factory(factory);

  auto impl = Imgajz1LibraryImpl::create(*jsonImpl);

  ASSERT_NE(impl, nullptr);
  EXPECT_EQ(impl->get_app_name(), "MyAnnotator");
  EXPECT_EQ(impl->get_app_version(), "1.2.3");
  EXPECT_EQ(impl->get_records_factory(), factory);
}

TEST_F(UTEST_Imgajz1LibraryImpl, tells_the_imgajz1_path_by_its_extension)
{
  EXPECT_TRUE(Imgajz1LibraryImpl::is_imgajz1_path("project.imgajz1"));
  EXPECT_TRUE(
      Imgajz1LibraryImpl::is_imgajz1_path("/home/user/cars/project.imgajz1"));
  EXPECT_TRUE(Imgajz1LibraryImpl::is_imgajz1_path("project.IMGAJZ1"));
  EXPECT_TRUE(Imgajz1LibraryImpl::is_imgajz1_path("project.Imgajz1"));

  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("project.json"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("project.imgajz1.json"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("project.imgajz2"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("project"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("imgajz1"));
  EXPECT_FALSE(
      Imgajz1LibraryImpl::is_imgajz1_path("/data.imgajz1/project.json"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path(""));
}

TEST_F(UTEST_Imgajz1LibraryImpl, reads_the_json_project_file_compressed_by_hand)
{
  // the gzip -c project.json > project.imgajz1 of the stock gzip tools
  write_compressed(json_project());

  ImageRecordsSet loaded;

  ASSERT_TRUE(loads(loaded));
  EXPECT_TRUE(ImageRecord::equal(loaded, one_record()));
}

TEST_F(UTEST_Imgajz1LibraryImpl, reads_the_json_project_file_of_no_compression)
{
  write_file(json_project());

  ASSERT_FALSE(ZlibController::is_gzip_file(project.string()));

  ImageRecordsSet loaded;

  ASSERT_TRUE(loads(loaded));
  EXPECT_TRUE(ImageRecord::equal(loaded, one_record()));
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_a_file_which_is_not_a_json)
{
  write_compressed("this is not a json at all");

  ImageRecordsSet loaded;

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());

  write_file("this is not a json either");

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_a_json_of_no_project)
{
  write_compressed("{\"user\": {\"name\": \"nobody\"}}");

  ImageRecordsSet loaded;

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_an_empty_file)
{
  write_file("");

  ImageRecordsSet loaded;

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());

  write_compressed("");

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_a_file_which_is_not_there)
{
  ImageRecordsSet loaded;

  EXPECT_FALSE(Imgajz1LibraryImpl::create()->load(
      (root / "absent.imgajz1").string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_the_truncated_file)
{
  ASSERT_TRUE(
      Imgajz1LibraryImpl::create()->store(project.string(), one_record()));

  const std::string whole = read_file(project);

  for (const std::size_t size :
       {whole.size() / 2U, whole.size() - 8U, whole.size() - 1U}) {
    write_file(whole.substr(0U, size));

    ImageRecordsSet loaded;

    EXPECT_FALSE(loads(loaded)) << size << " of " << whole.size() << " bytes";
    EXPECT_TRUE(loaded.empty());
  }
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_the_file_of_a_corrupted_trailer)
{
  ASSERT_TRUE(
      Imgajz1LibraryImpl::create()->store(project.string(), one_record()));

  // the CRC32 of the data starts the 8 bytes of the gzip trailer, the JSON
  // text itself decompresses complete
  std::string corrupted = read_file(project);

  corrupted[corrupted.size() - 8U] =
      static_cast<char>(corrupted[corrupted.size() - 8U] ^ 0x5A);

  write_file(corrupted);

  ImageRecordsSet loaded;

  EXPECT_FALSE(loads(loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_Imgajz1LibraryImpl, refuses_a_destination_unable_to_be_written)
{
  EXPECT_FALSE(Imgajz1LibraryImpl::create()->store(
      (root / "absent-dir" / "project.imgajz1").string(), one_record()));
  EXPECT_FALSE(fs::exists(root / "absent-dir"));
}

TEST_F(UTEST_Imgajz1LibraryImpl, keeps_the_file_of_a_string_of_no_utf8)
{
  auto impl = Imgajz1LibraryImpl::create();

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  const std::string before = read_file(project);

  ImageRecordsSet latin1 = one_record();

  latin1.front()->rects.front()->name = "caf\xE9";

  EXPECT_FALSE(impl->store(project.string(), latin1));
  EXPECT_EQ(read_file(project), before);
}
