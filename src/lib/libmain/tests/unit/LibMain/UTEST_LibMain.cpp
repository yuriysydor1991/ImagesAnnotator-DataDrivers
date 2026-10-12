#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "IADataDriversContext.h"
#include "IAnnotationsDB.h"
#include "src/lib/libmain/LibFactory.h"
#include "src/lib/libmain/LibMain.h"

using namespace ImagesAnnotatorDataDrivers013;
using namespace lib0impl;
using namespace testing;

namespace
{

/**
 * @brief The inline gmock stand-in for the annotations database the LibMain
 * builds through the (also mocked) LibFactory.
 */
class AnnotationsDBMock : public IAnnotationsDB
{
 public:
  virtual ~AnnotationsDBMock() = default;
  AnnotationsDBMock()
  {
    ON_CALL(*this, get_images_db()).WillByDefault(ReturnRef(records));
  }

  MOCK_METHOD(bool, load_db, (const std::string& fpath), (override));
  MOCK_METHOD(bool, store_db, (), (override));
  MOCK_METHOD(bool, store_db, (const std::string& fpath), (override));
  MOCK_METHOD(void, add_images_db, (const ImageRecordsSet& andb), (override));
  MOCK_METHOD(void, set_app_info,
              (const std::string& name, const std::string& version),
              (override));
  MOCK_METHOD(std::string, get_app_name, (), (const, override));
  MOCK_METHOD(std::string, get_app_version, (), (const, override));

  MOCK_METHOD(ImageRecordsSet&, get_images_db, (), (override));
  MOCK_METHOD(std::string, get_db_path, (), (override));
  MOCK_METHOD(AnnotationsList, get_available_annotations, (), (override));
  MOCK_METHOD(bool, changed, (), (override));
  MOCK_METHOD(bool, delete_image_record, (const std::string& irFullPath),
              (override));

  /// @brief The set the non-const get_images_db() reference is taken from
  ImageRecordsSet records;
};

using AnnotationsDBMockPtr = std::shared_ptr<NiceMock<AnnotationsDBMock>>;

}  // namespace

class UTEST_LibMain : public Test
{
 public:
  UTEST_LibMain()
      : libmain{std::make_shared<LibMain>()},
        db{std::make_shared<NiceMock<AnnotationsDBMock>>()}
  {
  }

  void TearDown() override { LibFactory::onMockCreate = nullptr; }

  /**
   * @brief Arms the LibFactory mock so that the next created instance hands
   * the fixture database out of its context taking create_annotations_db.
   */
  void expect_db_creation(MockFunction<void(LibFactory&)>& ensurer)
  {
    auto theDB = db;

    EXPECT_CALL(ensurer, Call)
        .Times(1)
        .WillOnce(Invoke([theDB](LibFactory& instance) {
          EXPECT_CALL(instance, create_annotations_db(_))
              .Times(1)
              .WillOnce(Return(theDB));
        }));

    LibFactory::onMockCreate = ensurer.AsStdFunction();
  }

  std::shared_ptr<LibMain> libmain;
  AnnotationsDBMockPtr db;
};

TEST_F(UTEST_LibMain, libcall_no_context_failure)
{
  EXPECT_FALSE(libmain->libcall({}));
}

TEST_F(UTEST_LibMain, libcall_no_db_path_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  expect_db_creation(mockEnsurer);

  EXPECT_CALL(*db, load_db(_)).Times(0);

  auto ctx = std::make_shared<IADataDriversContext>();

  EXPECT_TRUE(libmain->libcall(ctx));
  EXPECT_EQ(ctx->db, db);
}

TEST_F(UTEST_LibMain, libcall_with_db_path_success)
{
  const std::string dbPath{"/some/where/the-project-db.json"};

  MockFunction<void(LibFactory&)> mockEnsurer;

  expect_db_creation(mockEnsurer);

  EXPECT_CALL(*db, load_db(dbPath)).Times(1).WillOnce(Return(true));

  auto ctx = std::make_shared<IADataDriversContext>();

  ctx->db_path = dbPath;

  EXPECT_TRUE(libmain->libcall(ctx));
  EXPECT_EQ(ctx->db, db);
}

TEST_F(UTEST_LibMain, libcall_db_load_failure)
{
  const std::string dbPath{"/no/such/the-project-db.json"};

  MockFunction<void(LibFactory&)> mockEnsurer;

  expect_db_creation(mockEnsurer);

  EXPECT_CALL(*db, load_db(dbPath)).Times(1).WillOnce(Return(false));

  auto ctx = std::make_shared<IADataDriversContext>();

  ctx->db_path = dbPath;

  EXPECT_FALSE(libmain->libcall(ctx));
  EXPECT_EQ(ctx->db, nullptr);
}

TEST_F(UTEST_LibMain, libcall_no_db_created_failure)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db(_))
            .Times(1)
            .WillOnce(Return(nullptr));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto ctx = std::make_shared<IADataDriversContext>();

  EXPECT_FALSE(libmain->libcall(ctx));
  EXPECT_EQ(ctx->db, nullptr);
}

TEST_F(UTEST_LibMain, create_success) { EXPECT_NE(LibMain::create(), nullptr); }
