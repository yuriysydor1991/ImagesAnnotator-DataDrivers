#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "ImageRecordSegment.h"
#include "src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h"
#include "src/dbs/AnnotationsDBs/Imgajz2LibraryImpl.h"
#include "src/dbs/AnnotationsDBs/LibraryImpl.h"
#include "src/records/RecordsFactory.h"
#include "src/zlib/ZlibController.h"

using namespace testing;
using iadd0zlibi::ZlibController;
using iannotator::dbs::annotations::Imgajz1LibraryImpl;
using iannotator::dbs::annotations::Imgajz2LibraryImpl;
using iannotator::dbs::annotations::LibraryImpl;
using ImagesAnnotatorDataDrivers015::ImageRecord;
using ImagesAnnotatorDataDrivers015::ImageRecordRect;
using ImagesAnnotatorDataDrivers015::ImageRecordSegment;
using ImagesAnnotatorDataDrivers015::ImageRecordsSet;

namespace fs = std::filesystem;

namespace
{

class UTEST_Imgajz2LibraryImpl : public Test
{
 public:
  void SetUp() override
  {
    root = fs::path{TempDir()} / "utest_imgajz2_library_impl";

    fs::remove_all(root);
    fs::create_directories(root);

    project = root / "project.imgajz2";
  }

  void TearDown() override { fs::remove_all(root); }

  static ImageRecordsSet records()
  {
    auto ir = std::make_shared<ImageRecord>("a.png", "/images");

    ir->iwidth = 640;
    ir->iheight = 480;
    ir->rects.emplace_back(
        std::make_shared<ImageRecordRect>("dog", 50, 20, 100, 40));
    ir->rects.emplace_back(std::make_shared<ImageRecordSegment>(
        "cat", 10, 20, 30, 40,
        ImageRecordRect::Points{{0.0, 0.0}, {1.0, 0.5}, {0.25, 1.0}}));

    return ImageRecordsSet{ir};
  }

  nlohmann::json document() const
  {
    const auto text =
        ZlibController::create()->uncompress_from_file(project.string());

    return nlohmann::json::parse(text.cbegin(), text.cend());
  }

  fs::path root;
  fs::path project;
};

}  // namespace

TEST_F(UTEST_Imgajz2LibraryImpl, stores_and_loads_the_segments_back)
{
  ASSERT_TRUE(Imgajz2LibraryImpl::create()->store(project.string(), records()));

  ImageRecordsSet loaded;

  ASSERT_TRUE(Imgajz2LibraryImpl::create()->load(project.string(), loaded));

  EXPECT_TRUE(ImageRecord::equal(loaded, records()));
  ASSERT_EQ(loaded.front()->rects.size(), 2U);
  EXPECT_NE(std::dynamic_pointer_cast<ImageRecordSegment>(
                loaded.front()->rects.back()),
            nullptr);
}

TEST_F(UTEST_Imgajz2LibraryImpl, writes_the_points_next_to_the_rect)
{
  ASSERT_TRUE(Imgajz2LibraryImpl::create()->store(project.string(), records()));

  ASSERT_TRUE(ZlibController::is_gzip_file(project.string()));

  const auto rects =
      document()["annotations"][0]["annotations"][0]["annotations"];

  EXPECT_FALSE(rects[0].contains("segment"));
  EXPECT_EQ(rects[1]["rect"]["width"], 30);
  EXPECT_EQ(rects[1]["segment"],
            nlohmann::json::parse("[[0.0, 0.0], [1.0, 0.5], [0.25, 1.0]]"));
}

TEST_F(UTEST_Imgajz2LibraryImpl, declares_the_format_and_its_annotation_types)
{
  ASSERT_TRUE(Imgajz2LibraryImpl::create()->store(project.string(), {}));

  EXPECT_EQ(document()["format"],
            nlohmann::json::parse(
                R"({"name": "imgajz2", "types": ["rect", "segment"]})"));
}

TEST_F(UTEST_Imgajz2LibraryImpl, reads_the_imgajz1_project_of_rectangles)
{
  const auto imgajz1 = (root / "project.imgajz1").string();
  auto rects = records();

  rects.front()->rects.pop_back();

  ASSERT_TRUE(Imgajz1LibraryImpl::create()->store(imgajz1, rects));

  ImageRecordsSet loaded;

  ASSERT_TRUE(Imgajz2LibraryImpl::create()->load(imgajz1, loaded));
  EXPECT_TRUE(ImageRecord::equal(loaded, rects));
}

TEST_F(UTEST_Imgajz2LibraryImpl, older_formats_refuse_the_segments)
{
  const auto json = root / "project.json";
  const auto imgajz1 = root / "project.imgajz1";

  EXPECT_FALSE(LibraryImpl::create()->store(json.string(), records()));
  EXPECT_FALSE(
      Imgajz1LibraryImpl::create()->store(imgajz1.string(), records()));

  EXPECT_FALSE(fs::exists(json));
  EXPECT_FALSE(fs::exists(imgajz1));
}

TEST_F(UTEST_Imgajz2LibraryImpl, older_formats_read_the_rect_of_a_segment)
{
  ASSERT_TRUE(Imgajz2LibraryImpl::create()->store(project.string(), records()));

  ImageRecordsSet loaded;

  ASSERT_TRUE(Imgajz1LibraryImpl::create()->load(project.string(), loaded));
  ASSERT_EQ(loaded.front()->rects.size(), 2U);
  EXPECT_TRUE(ImageRecordRect::equal(
      loaded.front()->rects.back(),
      std::make_shared<ImageRecordRect>("cat", 10, 20, 30, 40)));
}

TEST_F(UTEST_Imgajz2LibraryImpl, takes_the_settings_of_the_given_driver)
{
  auto like = LibraryImpl::create();
  auto factory = iannotator::records::RecordsFactory::create();

  like->set_app_info("MyTool", "1.2.3");
  like->set_records_factory(factory);

  const auto impl = Imgajz2LibraryImpl::create(*like);

  EXPECT_EQ(impl->get_app_name(), "MyTool");
  EXPECT_EQ(impl->get_app_version(), "1.2.3");
  EXPECT_EQ(impl->get_records_factory(), factory);
}

TEST_F(UTEST_Imgajz2LibraryImpl, tells_the_imgajz2_path_by_its_extension)
{
  EXPECT_TRUE(Imgajz2LibraryImpl::is_imgajz2_path("/a/project.imgajz2"));
  EXPECT_TRUE(Imgajz2LibraryImpl::is_imgajz2_path("project.IMGAJZ2"));
  EXPECT_FALSE(Imgajz2LibraryImpl::is_imgajz2_path("project.imgajz1"));
  EXPECT_FALSE(Imgajz2LibraryImpl::is_imgajz2_path("project.json"));
  EXPECT_FALSE(Imgajz1LibraryImpl::is_imgajz1_path("project.imgajz2"));
}
