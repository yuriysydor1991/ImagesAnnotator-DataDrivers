#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "LibraryContext.h"
#include "project-global-decls.h"
#include "src/lib/libmain/LibFactory.h"

using namespace ImagesAnnotatorDataDrivers011;
using namespace lib0impl;
using namespace testing;

class UTEST_LibFactory : public Test
{
 public:
  UTEST_LibFactory() : factory{std::make_shared<LibFactory>()} {}

  std::shared_ptr<LibFactory> factory;
};

TEST_F(UTEST_LibFactory, create_default_lib_success)
{
  EXPECT_NE(factory->create_default_lib(), nullptr);
}

TEST_F(UTEST_LibFactory, create_default_context_success)
{
  EXPECT_NE(factory->create_default_context(), nullptr);
}

TEST_F(UTEST_LibFactory, create_appropriate_lib_success)
{
  EXPECT_NE(factory->create_appropriate_lib({}), nullptr);
}

TEST_F(UTEST_LibFactory, create_factory_success)
{
  EXPECT_NE(LibFactory::create_factory(), nullptr);
}

TEST_F(UTEST_LibFactory, create_default_context_defaults_success)
{
  auto ctx = factory->create_default_context();

  ASSERT_NE(ctx, nullptr);

  EXPECT_EQ(ctx->app_name, project_decls::ANNOTATIONS_DB_ROOT_APP_NAME);
  EXPECT_EQ(ctx->app_version, project_decls::ANNOTATIONS_DB_APP_VERSION);
  EXPECT_TRUE(ctx->db_path.empty());
  EXPECT_EQ(ctx->db, nullptr);
}

TEST_F(UTEST_LibFactory, create_annotations_db_success)
{
  auto db = factory->create_annotations_db();

  ASSERT_NE(db, nullptr);

  EXPECT_EQ(db->get_app_name(), project_decls::ANNOTATIONS_DB_ROOT_APP_NAME);
  EXPECT_EQ(db->get_app_version(), project_decls::ANNOTATIONS_DB_APP_VERSION);
  EXPECT_TRUE(db->get_images_db().empty());
}

TEST_F(UTEST_LibFactory, create_annotations_db_with_context_success)
{
  const std::string customName{"TheCustomApplicationName"};
  const std::string customVersion{"7.77.777"};

  auto ctx = LibraryContext::create();

  ASSERT_NE(ctx, nullptr);

  ctx->app_name = customName;
  ctx->app_version = customVersion;
  ctx->records_factory = factory->create_records_factory();
  ctx->merger = factory->create_db_merger();

  auto db = factory->create_annotations_db(ctx);

  ASSERT_NE(db, nullptr);

  EXPECT_EQ(db->get_app_name(), customName);
  EXPECT_EQ(db->get_app_version(), customVersion);
}

TEST_F(UTEST_LibFactory, create_annotations_db_no_context_defaults_success)
{
  auto db = factory->create_annotations_db(nullptr);

  ASSERT_NE(db, nullptr);

  EXPECT_EQ(db->get_app_name(), project_decls::ANNOTATIONS_DB_ROOT_APP_NAME);
  EXPECT_EQ(db->get_app_version(), project_decls::ANNOTATIONS_DB_APP_VERSION);
}

TEST_F(UTEST_LibFactory, create_records_factory_success)
{
  auto records = factory->create_records_factory();

  ASSERT_NE(records, nullptr);

  EXPECT_NE(records->create_image_record(), nullptr);
}

TEST_F(UTEST_LibFactory, create_db_merger_success)
{
  auto merger = factory->create_db_merger();

  ASSERT_NE(merger, nullptr);

  ImageRecordsSet dst;
  ImageRecordsSet append;

  merger->merge(dst, append);

  EXPECT_TRUE(dst.empty());
}
