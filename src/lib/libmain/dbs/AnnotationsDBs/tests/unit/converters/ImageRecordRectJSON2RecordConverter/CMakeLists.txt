cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImageRecordRectJSON2RecordConverter
  UTEST_ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_ImageRecordRectJSON2RecordConverter
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_ImageRecordRectJSON2RecordConverter
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImageRecordRectJSON2RecordConverter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
