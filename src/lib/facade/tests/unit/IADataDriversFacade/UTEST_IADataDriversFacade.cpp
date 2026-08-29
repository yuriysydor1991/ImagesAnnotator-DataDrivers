#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "IADataDriversContext.h"
#include "IADataDriversFacade.h"
#include "IAnnotationsDB.h"
#include "project-global-decls.h"
#include "src/lib/libmain/LibFactory.h"
#include "src/lib/libmain/LibMain.h"

using namespace ImagesAnnotatorDataDrivers012;
using namespace lib0impl;
using namespace testing;

namespace
{

/**
 * @brief The inline gmock stand-in for the annotations database the facade
 * obtains from the (also mocked) LibFactory.
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

class UTEST_IADataDriversFacade : public Test
{
 public:
  using LibFactory = lib0impl::LibFactory;

  UTEST_IADataDriversFacade()
      : db{std::make_shared<NiceMock<AnnotationsDBMock>>()}
  {
  }
  ~UTEST_IADataDriversFacade() = default;

  void TearDown() override { LibFactory::onMockCreate = nullptr; }

  AnnotationsDBMockPtr db;
};

TEST_F(UTEST_IADataDriversFacade, create_default_lib_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_appropriate_lib(_)).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = IADataDriversFacade::create_library({});
}

TEST_F(UTEST_IADataDriversFacade, create_default_lib_direct_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_default_lib()).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = IADataDriversFacade::create_default_lib();
}

TEST_F(UTEST_IADataDriversFacade, create_library_context_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_default_context()).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = IADataDriversFacade::create_library_context();
}

TEST_F(UTEST_IADataDriversFacade, create_annotations_db_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  auto theDB = db;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([theDB](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db())
            .Times(1)
            .WillOnce(Return(theDB));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  EXPECT_EQ(IADataDriversFacade::create_annotations_db(), db);
}

TEST_F(UTEST_IADataDriversFacade, create_annotations_db_with_context_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  auto ctx = IADataDriversContext::create();

  ASSERT_NE(ctx, nullptr);

  auto theDB = db;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([theDB, ctx](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db(Eq(ctx)))
            .Times(1)
            .WillOnce(Return(theDB));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  EXPECT_EQ(IADataDriversFacade::create_annotations_db(ctx), db);
}

TEST_F(UTEST_IADataDriversFacade, open_annotations_db_success)
{
  const std::string dbPath{"/some/where/the-project-db.json"};

  MockFunction<void(LibFactory&)> mockEnsurer;

  auto theDB = db;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([theDB](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db())
            .Times(1)
            .WillOnce(Return(theDB));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  EXPECT_CALL(*db, load_db(dbPath)).Times(1).WillOnce(Return(true));

  EXPECT_EQ(IADataDriversFacade::open_annotations_db(dbPath), db);
}

TEST_F(UTEST_IADataDriversFacade, open_annotations_db_load_failure)
{
  const std::string dbPath{"/no/such/the-project-db.json"};

  MockFunction<void(LibFactory&)> mockEnsurer;

  auto theDB = db;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([theDB](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db())
            .Times(1)
            .WillOnce(Return(theDB));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  EXPECT_CALL(*db, load_db(dbPath)).Times(1).WillOnce(Return(false));

  EXPECT_EQ(IADataDriversFacade::open_annotations_db(dbPath), nullptr);
}

TEST_F(UTEST_IADataDriversFacade, open_annotations_db_no_db_failure)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_annotations_db())
            .Times(1)
            .WillOnce(Return(nullptr));
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  EXPECT_EQ(IADataDriversFacade::open_annotations_db("whatever.json"), nullptr);
}

TEST_F(UTEST_IADataDriversFacade, create_records_factory_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_records_factory()).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = IADataDriversFacade::create_records_factory();
}

TEST_F(UTEST_IADataDriversFacade, create_db_merger_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_db_merger()).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = IADataDriversFacade::create_db_merger();
}

TEST_F(UTEST_IADataDriversFacade, library_version_success)
{
  EXPECT_EQ(IADataDriversFacade::library_version(),
            project_decls::PROJECT_BUILD_VERSION);
}

TEST_F(UTEST_IADataDriversFacade, default_db_root_app_name_success)
{
  EXPECT_EQ(IADataDriversFacade::default_db_root_app_name(),
            project_decls::ANNOTATIONS_DB_ROOT_APP_NAME);
}

TEST_F(UTEST_IADataDriversFacade, default_db_app_version_success)
{
  EXPECT_EQ(IADataDriversFacade::default_db_app_version(),
            project_decls::ANNOTATIONS_DB_APP_VERSION);
}
