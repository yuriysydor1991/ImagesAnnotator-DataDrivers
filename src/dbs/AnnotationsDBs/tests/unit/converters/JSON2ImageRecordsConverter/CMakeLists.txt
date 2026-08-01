cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_JSON2ImageRecordsConverter
  UTEST_JSON2ImageRecordsConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/JSON2ImageRecordsConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_JSON2ImageRecordsConverter
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_JSON2ImageRecordsConverter
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_JSON2ImageRecordsConverter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
