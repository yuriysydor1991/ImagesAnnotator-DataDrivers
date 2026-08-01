#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

#include "IAnnotationsDB.h"
#include "IImagesPathsDBProvider.h"
#include "ILib.h"
#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "LibraryContext.h"
#include "LibraryFacade.h"
#include "ctests_decls.h"

using namespace testing;

/// @brief The very alias every downstream project is encouraged to introduce
namespace iadd = ImagesAnnotatorDataDrivers011;

namespace
{

/**
 * @brief The EXPECT_EXIT predicate rejecting a successful libcall only.
 *
 * The produced shared library keeps its asserts enabled, so the nullptr
 * context guard of ILib::libcall aborts the process before its false return
 * value may be observed, while an NDEBUG library build returns that false
 * instead. Both outcomes are a refusal of the invalid call and only a success
 * is a defect, hence the child process is merely required not to leave with
 * the zero exit code.
 */
class LibcallDidNotSucceed
{
 public:
  bool operator()(int exitStatus) const
  {
    return !ExitedWithCode(EXIT_SUCCESS)(exitStatus);
  }
};

}  // namespace

/**
 * @brief The annotations directory database component test.
 *
 * It links the real produced shared library and reaches it through the
 * installable public headers exclusively, which makes it the proof that a
 * downstream project is able to do the whole job with those headers alone.
 */
class CTEST_AnnotationsDirDB : public Test
{
 public:
  inline static const std::string tmp_data_dir =
      ctests_decls::tmp_data_path_root + "/CTEST_AnnotationsDirDB";
  inline static const std::string ctest_data_dir = CTEST_DATA_ROOT;

  inline static const std::string ctest_data_json_plan_name =
      "basic-structure-plan.json";
  inline static const std::string nonexist_ctest_data_json_plan =
      "nonexistent-annotations-db-file.not-a-json";
  inline static const std::string serialize_failure_json_name =
      "non_empty_annotation_db_init_serialize_failure.json";

  /// @brief The JSON root element name the produced project files carry
  inline static const std::string expected_root_app_name = "ImagesAnnotator";

  /// @brief The image records count the basic structure plan holds
  inline static constexpr const std::size_t plan_records_count = 5U;

  ~CTEST_AnnotationsDirDB() override = default;
  CTEST_AnnotationsDirDB() = default;

  void SetUp() override
  {
    std::error_code ec;

    std::filesystem::create_directories(tmp_data_dir, ec);

    ASSERT_FALSE(static_cast<bool>(ec));
    ASSERT_TRUE(std::filesystem::is_directory(tmp_data_dir));
  }

  void TearDown() override
  {
    std::error_code ec;

    std::filesystem::remove_all(tmp_data_dir, ec);
  }

  static std::string ctest_data_path(const std::string& dbname)
  {
    return ctest_data_dir + "/" + dbname;
  }

  static std::string tmp_data_path(const std::string& dbname)
  {
    return tmp_data_dir + "/" + dbname;
  }

  /**
   * @brief Builds a default library context pointed at the given fixture file.
   */
  static iadd::LibraryContextPtr make_context(const std::string& dbname)
  {
    auto ctx = iadd::LibraryFacade::create_library_context();

    if (ctx != nullptr && !dbname.empty()) {
      ctx->db_path = ctest_data_path(dbname);
    }

    return ctx;
  }

  /**
   * @brief Opens the basic structure plan fixture through the public facade.
   */
  static iadd::IAnnotationsDBPtr open_plan_db()
  {
    return iadd::LibraryFacade::open_annotations_db(
        ctest_data_path(ctest_data_json_plan_name));
  }

  static std::string read_whole_file(const std::string& fpath)
  {
    std::ifstream file(fpath);
    std::stringstream content;

    content << file.rdbuf();

    return content.str();
  }
};

TEST_F(CTEST_AnnotationsDirDB, no_context_init_failure)
{
  EXPECT_EXIT(
      {
        auto lib = iadd::LibraryFacade::create_default_lib();

        std::exit(lib != nullptr && lib->libcall(nullptr) ? EXIT_SUCCESS
                                                          : EXIT_FAILURE);
      },
      LibcallDidNotSucceed{}, "");
}

TEST_F(CTEST_AnnotationsDirDB, empty_context_init_success)
{
  auto lib = iadd::LibraryFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = iadd::LibraryFacade::create_library_context();

  ASSERT_NE(ctx, nullptr);
  ASSERT_TRUE(ctx->db_path.empty());

  EXPECT_TRUE(lib->libcall(ctx));

  ASSERT_NE(ctx->db, nullptr);
  EXPECT_TRUE(ctx->db->get_images_db().empty());
  EXPECT_TRUE(ctx->db->get_db_path().empty());
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_invalid_annotation_db_init_failure)
{
  auto lib = iadd::LibraryFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = make_context(nonexist_ctest_data_json_plan);

  ASSERT_NE(ctx, nullptr);

  EXPECT_FALSE(lib->libcall(ctx));
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_annotation_db_init_serialize_failure)
{
  auto lib = iadd::LibraryFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = make_context(serialize_failure_json_name);

  ASSERT_NE(ctx, nullptr);

  EXPECT_FALSE(lib->libcall(ctx));
}

TEST_F(CTEST_AnnotationsDirDB, basic_json_plan_db_init_success)
{
  auto lib = iadd::LibraryFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = make_context(ctest_data_json_plan_name);

  ASSERT_NE(ctx, nullptr);

  EXPECT_TRUE(lib->libcall(ctx));

  ASSERT_NE(ctx->db, nullptr);
  EXPECT_FALSE(ctx->db->get_images_db().empty());
  EXPECT_EQ(ctx->db->get_images_db().size(), plan_records_count);
}

TEST_F(CTEST_AnnotationsDirDB, opened_plan_db_is_unchanged_and_knows_its_path)
{
  const std::string dbpath = ctest_data_path(ctest_data_json_plan_name);

  auto db = iadd::LibraryFacade::open_annotations_db(dbpath);

  ASSERT_NE(db, nullptr);

  EXPECT_EQ(db->get_db_path(), dbpath);
  EXPECT_FALSE(db->changed());
  EXPECT_EQ(db->get_images_db().size(), plan_records_count);

  EXPECT_EQ(db->get_app_name(), expected_root_app_name);
  EXPECT_EQ(db->get_app_name(),
            iadd::LibraryFacade::default_db_root_app_name());
  EXPECT_EQ(db->get_app_version(),
            iadd::LibraryFacade::default_db_app_version());
}

TEST_F(CTEST_AnnotationsDirDB, opened_plan_db_lists_every_used_annotation)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const iadd::IImagesPathsDBProvider::AnnotationsList expected{
      "annotation-tag-or-class-name-1", "annotation-tag-or-class-name-2",
      "annotation-tag-or-class-name-3", "annotation-tag-or-class-name-4"};

  EXPECT_EQ(db->get_available_annotations(), expected);
}

TEST_F(CTEST_AnnotationsDirDB, appended_rectangle_marks_the_db_changed)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);
  ASSERT_FALSE(db->changed());

  auto factory = iadd::LibraryFacade::create_records_factory();

  ASSERT_NE(factory, nullptr);

  auto rect =
      factory->create_image_rect_record("brand-new-annotation", 1, 2, 3, 4);

  ASSERT_NE(rect, nullptr);

  auto& records = db->get_images_db();

  ASSERT_FALSE(records.empty());
  ASSERT_NE(records.front(), nullptr);

  records.front()->rects.emplace_back(rect);

  EXPECT_TRUE(db->changed());
  EXPECT_EQ(db->get_available_annotations().count("brand-new-annotation"), 1U);
}

TEST_F(CTEST_AnnotationsDirDB, store_and_reopen_round_trip_keeps_the_records)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string stored_path = tmp_data_path("round-trip-db.json");

  ASSERT_TRUE(db->store_db(stored_path));

  EXPECT_EQ(db->get_db_path(), stored_path);
  EXPECT_FALSE(db->changed());
  ASSERT_TRUE(std::filesystem::exists(stored_path));

  auto reopened = iadd::LibraryFacade::open_annotations_db(stored_path);

  ASSERT_NE(reopened, nullptr);

  EXPECT_EQ(reopened->get_db_path(), stored_path);
  EXPECT_FALSE(reopened->changed());
  EXPECT_EQ(reopened->get_images_db().size(), plan_records_count);

  EXPECT_TRUE(
      iadd::ImageRecord::equal(db->get_images_db(), reopened->get_images_db()));
  EXPECT_EQ(reopened->get_available_annotations(),
            db->get_available_annotations());
}

TEST_F(CTEST_AnnotationsDirDB, stored_file_is_rooted_at_the_app_name_element)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string stored_path = tmp_data_path("rooted-db.json");

  ASSERT_TRUE(db->store_db(stored_path));

  // The target links no JSON library on purpose - a downstream project needs
  // none - so the produced file is inspected as a plain text here.
  const std::string content = read_whole_file(stored_path);

  ASSERT_FALSE(content.empty());

  EXPECT_NE(content.find("\"" + expected_root_app_name + "\""),
            std::string::npos);
  EXPECT_NE(content.find("\"" + db->get_app_version() + "\""),
            std::string::npos);
  EXPECT_NE(content.find("\"annotations\""), std::string::npos);
}

TEST_F(CTEST_AnnotationsDirDB, delete_image_record_shrinks_the_records_set)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);
  ASSERT_EQ(db->get_images_db().size(), plan_records_count);
  ASSERT_NE(db->get_images_db().front(), nullptr);

  const std::string doomed = db->get_images_db().front()->get_full_path();

  ASSERT_FALSE(doomed.empty());

  EXPECT_TRUE(db->delete_image_record(doomed));

  EXPECT_EQ(db->get_images_db().size(), plan_records_count - 1U);
  EXPECT_TRUE(db->changed());

  for (const auto& ir : db->get_images_db()) {
    ASSERT_NE(ir, nullptr);
    EXPECT_NE(ir->get_full_path(), doomed);
  }
}

TEST_F(CTEST_AnnotationsDirDB, open_annotations_db_of_a_missing_file_fails)
{
  EXPECT_EQ(iadd::LibraryFacade::open_annotations_db(
                ctest_data_path(nonexist_ctest_data_json_plan)),
            nullptr);
}

TEST_F(CTEST_AnnotationsDirDB, open_annotations_db_of_an_alien_json_fails)
{
  EXPECT_EQ(iadd::LibraryFacade::open_annotations_db(
                ctest_data_path(serialize_failure_json_name)),
            nullptr);
}
