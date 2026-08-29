cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_LibFactory
  UTEST_LibFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/LibFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/LibMain.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/facade/IADataDriversContext.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/AnnotationsDirDB.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/AnnotationsJSONSerializator.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/mergers/DefaultDBMerger.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecords2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/JSON2ImageRecordsConverter.cpp
  ${IADD_RECORDS_SRC}
  ${IADD_HELPERS_SRC}
  ${IADD_CURL_SRC}
)

target_include_directories(
  UTEST_LibFactory
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_LibFactory
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json CURL::libcurl
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_LibFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
