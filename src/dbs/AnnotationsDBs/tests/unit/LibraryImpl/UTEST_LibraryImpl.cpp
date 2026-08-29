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

using namespace testing;
using iannotator::dbs::annotations::LibraryImpl;
using ImagesAnnotatorDataDrivers012::ImageRecord;
using ImagesAnnotatorDataDrivers012::ImageRecordRect;
using ImagesAnnotatorDataDrivers012::ImageRecordsSet;
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
