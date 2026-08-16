cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_LibraryImpl
  UTEST_LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/AnnotationsJSONSerializator.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecords2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/JSON2ImageRecordsConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_LibraryImpl
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_LibraryImpl
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_LibraryImpl
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
