#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/LibraryImpl.h"
#include "src/records/RecordsFactory.h"

using namespace testing;
using iannotator::dbs::annotations::LibraryImpl;
using ImagesAnnotatorDataDrivers013::ImageRecord;
using ImagesAnnotatorDataDrivers013::ImageRecordRect;
using ImagesAnnotatorDataDrivers013::ImageRecordsSet;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace fs = std::filesystem;

namespace
{

/**
 * @brief Drives the project file driver over real files in a temporary
 * directory, since reading and writing them is the whole of its job.
 */
class UTEST_LibraryImpl : public Test
{
 public:
  void SetUp() override
  {
    root = fs::path{TempDir()} / "utest_library_impl";
    imagesDir = root / "images";

    fs::remove_all(root);
    fs::create_directories(imagesDir);

    project = root / "project.json";
  }

  void TearDown() override { fs::remove_all(root); }

  ImageRecordsSet one_record() const
  {
    auto ir = std::make_shared<ImageRecord>("a.png", imagesDir.string());

    ir->rects.emplace_back(
        std::make_shared<ImageRecordRect>("dog", 50, 20, 100, 40));

    return ImageRecordsSet{ir};
  }

  void write_file(const std::string& content) const
  {
    std::ofstream f(project);
    f << content;
  }

  std::string read_project() const
  {
    std::ifstream f(project);
    std::string content((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    return content;
  }

  fs::path root;
  fs::path imagesDir;
  fs::path project;
};

}  // namespace

TEST_F(UTEST_LibraryImpl, stores_and_loads_the_records_back)
{
  auto impl = LibraryImpl::create();

  ASSERT_NE(impl, nullptr);
  ASSERT_TRUE(impl->store(project.string(), one_record()));
  ASSERT_TRUE(fs::is_regular_file(project));

  ImageRecordsSet loaded;

  ASSERT_TRUE(impl->load(project.string(), loaded));
  ASSERT_EQ(loaded.size(), 1U);
  ASSERT_NE(loaded.front(), nullptr);

  EXPECT_EQ(loaded.front()->get_full_path(), (imagesDir / "a.png").string());
  ASSERT_EQ(loaded.front()->rects.size(), 1U);
  EXPECT_EQ(loaded.front()->rects.front()->name, "dog");
}

TEST_F(UTEST_LibraryImpl, stores_the_application_identity_it_was_given)
{
  auto impl = LibraryImpl::create();

  impl->set_app_info("MyAnnotator", "1.2.3");

  EXPECT_EQ(impl->get_app_name(), "MyAnnotator");
  EXPECT_EQ(impl->get_app_version(), "1.2.3");

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  const std::string content = read_project();

  EXPECT_NE(content.find("MyAnnotator"), std::string::npos);
  EXPECT_NE(content.find("1.2.3"), std::string::npos);
}

TEST_F(UTEST_LibraryImpl, loads_a_file_of_the_very_identity_it_carries)
{
  auto impl = LibraryImpl::create();

  impl->set_app_info("MyAnnotator", "1.2.3");

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  ImageRecordsSet loaded;

  EXPECT_TRUE(impl->load(project.string(), loaded));
  EXPECT_EQ(loaded.size(), 1U);
}

TEST_F(UTEST_LibraryImpl, refuses_a_file_of_another_root_element)
{
  auto impl = LibraryImpl::create();

  impl->set_app_info("MyAnnotator", "1.2.3");

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  auto other = LibraryImpl::create();

  other->set_app_info("SomebodyElse", "1.2.3");

  ImageRecordsSet loaded;

  EXPECT_FALSE(other->load(project.string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_LibraryImpl, refuses_a_file_without_the_annotations_element)
{
  auto impl = LibraryImpl::create();

  write_file(std::string{"{\""} + S::default_root_app_name + "\": {\"" +
             S::file_version_field_name + "\": \"" + S::default_app_version +
             "\"}}");

  ImageRecordsSet loaded;

  EXPECT_FALSE(impl->load(project.string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_LibraryImpl, refuses_a_file_which_is_not_there)
{
  auto impl = LibraryImpl::create();

  ImageRecordsSet loaded;

  EXPECT_FALSE(impl->load((root / "absent.json").string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_LibraryImpl, refuses_a_file_which_is_not_a_json)
{
  auto impl = LibraryImpl::create();

  write_file("this is not a json at all");

  ImageRecordsSet loaded;

  EXPECT_FALSE(impl->load(project.string(), loaded));
  EXPECT_TRUE(loaded.empty());
}

TEST_F(UTEST_LibraryImpl, reports_the_records_factory_it_was_given)
{
  auto impl = LibraryImpl::create();

  EXPECT_EQ(impl->get_records_factory(), nullptr);

  auto factory = iannotator::records::RecordsFactory::create();

  impl->set_records_factory(factory);

  EXPECT_EQ(impl->get_records_factory(), factory);
}

TEST_F(UTEST_LibraryImpl, keeps_the_file_of_a_string_of_no_utf8)
{
  auto impl = LibraryImpl::create();

  ASSERT_TRUE(impl->store(project.string(), one_record()));

  const std::string before = read_project();

  ImageRecordsSet latin1 = one_record();

  latin1.front()->rects.front()->name = "caf\xE9";

  EXPECT_FALSE(impl->store(project.string(), latin1));
  EXPECT_EQ(read_project(), before);
}

TEST_F(UTEST_LibraryImpl, refuses_a_destination_unable_to_be_written)
{
  auto impl = LibraryImpl::create();

  EXPECT_FALSE(impl->store((root / "absent-dir" / "project.json").string(),
                           one_record()));
  EXPECT_FALSE(fs::exists(root / "absent-dir"));
}
