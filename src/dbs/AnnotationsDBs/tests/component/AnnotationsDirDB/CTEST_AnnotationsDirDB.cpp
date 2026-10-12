#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

#include "IADataDriversContext.h"
#include "IADataDriversFacade.h"
#include "IADataDriversLib.h"
#include "IAnnotationsDB.h"
#include "IImagesPathsDBProvider.h"
#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "ctests_decls.h"

using namespace testing;

/// @brief The very alias every downstream project is encouraged to introduce
namespace iadd = ImagesAnnotatorDataDrivers012;

namespace
{

/**
 * @brief The EXPECT_EXIT predicate rejecting a successful libcall only.
 *
 * The produced shared library keeps its asserts enabled, so the nullptr
 * context guard of IADataDriversLib::libcall aborts the process before its
 * false return value may be observed, while an NDEBUG library build returns
 * that false instead. Both outcomes are a refusal of the invalid call and only
 * a success is a defect, hence the child process is merely required not to
 * leave with the zero exit code.
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
  static iadd::IADataDriversContextPtr make_context(const std::string& dbname)
  {
    auto ctx = iadd::IADataDriversFacade::create_library_context();

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
    return iadd::IADataDriversFacade::open_annotations_db(
        ctest_data_path(ctest_data_json_plan_name));
  }

  static std::string read_whole_file(const std::string& fpath)
  {
    std::ifstream file(fpath, std::ios::binary);
    std::stringstream content;

    content << file.rdbuf();

    return content.str();
  }

  /**
   * @brief Tells the gzip file by the ID1 and the ID2 bytes of the RFC 1952
   * member header it starts with.
   */
  static bool is_gzip_file(const std::string& fpath)
  {
    const std::string content = read_whole_file(fpath);

    return content.size() > 2U && content[0] == '\x1F' && content[1] == '\x8B';
  }
};

TEST_F(CTEST_AnnotationsDirDB, no_context_init_failure)
{
  EXPECT_EXIT(
      {
        auto lib = iadd::IADataDriversFacade::create_default_lib();

        std::exit(lib != nullptr && lib->libcall(nullptr) ? EXIT_SUCCESS
                                                          : EXIT_FAILURE);
      },
      LibcallDidNotSucceed{}, "");
}

TEST_F(CTEST_AnnotationsDirDB, empty_context_init_success)
{
  auto lib = iadd::IADataDriversFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = iadd::IADataDriversFacade::create_library_context();

  ASSERT_NE(ctx, nullptr);
  ASSERT_TRUE(ctx->db_path.empty());

  EXPECT_TRUE(lib->libcall(ctx));

  ASSERT_NE(ctx->db, nullptr);
  EXPECT_TRUE(ctx->db->get_images_db().empty());
  EXPECT_TRUE(ctx->db->get_db_path().empty());
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_invalid_annotation_db_init_failure)
{
  auto lib = iadd::IADataDriversFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = make_context(nonexist_ctest_data_json_plan);

  ASSERT_NE(ctx, nullptr);

  EXPECT_FALSE(lib->libcall(ctx));
}

TEST_F(CTEST_AnnotationsDirDB, non_empty_annotation_db_init_serialize_failure)
{
  auto lib = iadd::IADataDriversFacade::create_default_lib();

  ASSERT_NE(lib, nullptr);

  auto ctx = make_context(serialize_failure_json_name);

  ASSERT_NE(ctx, nullptr);

  EXPECT_FALSE(lib->libcall(ctx));
}

TEST_F(CTEST_AnnotationsDirDB, basic_json_plan_db_init_success)
{
  auto lib = iadd::IADataDriversFacade::create_default_lib();

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

  auto db = iadd::IADataDriversFacade::open_annotations_db(dbpath);

  ASSERT_NE(db, nullptr);

  EXPECT_EQ(db->get_db_path(), dbpath);
  EXPECT_FALSE(db->changed());
  EXPECT_EQ(db->get_images_db().size(), plan_records_count);

  EXPECT_EQ(db->get_app_name(), expected_root_app_name);
  EXPECT_EQ(db->get_app_name(),
            iadd::IADataDriversFacade::default_db_root_app_name());
  EXPECT_EQ(db->get_app_version(),
            iadd::IADataDriversFacade::default_db_app_version());
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

  auto factory = iadd::IADataDriversFacade::create_records_factory();

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

  auto reopened = iadd::IADataDriversFacade::open_annotations_db(stored_path);

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
  EXPECT_EQ(iadd::IADataDriversFacade::open_annotations_db(
                ctest_data_path(nonexist_ctest_data_json_plan)),
            nullptr);
}

TEST_F(CTEST_AnnotationsDirDB, open_annotations_db_of_an_alien_json_fails)
{
  EXPECT_EQ(iadd::IADataDriversFacade::open_annotations_db(
                ctest_data_path(serialize_failure_json_name)),
            nullptr);
}

TEST_F(CTEST_AnnotationsDirDB, imgajz1_store_and_reopen_keeps_the_records)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string stored_path = tmp_data_path("round-trip-db.imgajz1");

  ASSERT_TRUE(db->store_db(stored_path));

  EXPECT_EQ(db->get_db_path(), stored_path);
  EXPECT_FALSE(db->changed());
  EXPECT_TRUE(is_gzip_file(stored_path));

  auto reopened = iadd::IADataDriversFacade::open_annotations_db(stored_path);

  ASSERT_NE(reopened, nullptr);

  EXPECT_EQ(reopened->get_db_path(), stored_path);
  EXPECT_FALSE(reopened->changed());
  EXPECT_EQ(reopened->get_images_db().size(), plan_records_count);

  EXPECT_TRUE(
      iadd::ImageRecord::equal(db->get_images_db(), reopened->get_images_db()));
  EXPECT_EQ(reopened->get_available_annotations(),
            db->get_available_annotations());
}

TEST_F(CTEST_AnnotationsDirDB, imgajz1_file_is_smaller_than_the_json_one)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string json_path = tmp_data_path("sized-db.json");
  const std::string imgajz1_path = tmp_data_path("sized-db.imgajz1");

  ASSERT_TRUE(db->store_db(json_path));
  ASSERT_TRUE(db->store_db(imgajz1_path));

  EXPECT_LT(std::filesystem::file_size(imgajz1_path),
            std::filesystem::file_size(json_path));
}

TEST_F(CTEST_AnnotationsDirDB, the_file_name_extension_tells_the_stored_format)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string imgajz1_path = tmp_data_path("format-db.IMGAJZ1");
  const std::string json_path = tmp_data_path("format-db.json");

  ASSERT_TRUE(db->store_db(imgajz1_path));
  EXPECT_TRUE(is_gzip_file(imgajz1_path));

  // the argument less store goes on with the format of the path it writes
  ASSERT_TRUE(std::filesystem::remove(imgajz1_path));
  ASSERT_TRUE(db->store_db());
  EXPECT_TRUE(is_gzip_file(imgajz1_path));

  ASSERT_TRUE(db->store_db(json_path));
  EXPECT_FALSE(is_gzip_file(json_path));
  EXPECT_EQ(read_whole_file(json_path).front(), '{');

  auto reopened = iadd::IADataDriversFacade::open_annotations_db(json_path);

  ASSERT_NE(reopened, nullptr);
  EXPECT_TRUE(
      iadd::ImageRecord::equal(db->get_images_db(), reopened->get_images_db()));
}

TEST_F(CTEST_AnnotationsDirDB, imgajz1_file_keeps_the_configured_identity)
{
  auto ctx = iadd::IADataDriversFacade::create_library_context();

  ASSERT_NE(ctx, nullptr);

  ctx->app_name = "MyAnnotator";
  ctx->app_version = "2.0";

  auto db = iadd::IADataDriversFacade::create_annotations_db(ctx);
  auto plan = open_plan_db();

  ASSERT_NE(db, nullptr);
  ASSERT_NE(plan, nullptr);

  db->add_images_db(plan->get_images_db());

  const std::string stored_path = tmp_data_path("identity-db.imgajz1");

  ASSERT_TRUE(db->store_db(stored_path));

  // a database of another identity refuses the file
  EXPECT_EQ(iadd::IADataDriversFacade::open_annotations_db(stored_path),
            nullptr);

  auto same = iadd::IADataDriversFacade::create_annotations_db(ctx);

  ASSERT_NE(same, nullptr);
  ASSERT_TRUE(same->load_db(stored_path));
  EXPECT_EQ(same->get_images_db().size(), plan_records_count);
}

TEST_F(CTEST_AnnotationsDirDB, open_annotations_db_of_a_truncated_imgajz1_fails)
{
  auto db = open_plan_db();

  ASSERT_NE(db, nullptr);

  const std::string stored_path = tmp_data_path("truncated-db.imgajz1");

  ASSERT_TRUE(db->store_db(stored_path));

  const std::string whole = read_whole_file(stored_path);

  {
    std::ofstream file(stored_path, std::ios::binary | std::ios::trunc);
    file << whole.substr(0U, whole.size() - 4U);
  }

  EXPECT_EQ(iadd::IADataDriversFacade::open_annotations_db(stored_path),
            nullptr);
}
