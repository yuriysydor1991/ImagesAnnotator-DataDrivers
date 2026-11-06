cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImageRecordSegments2JSONConverter
  UTEST_ImageRecordSegments2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecords2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordSegments2JSONConverter.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_ImageRecordSegments2JSONConverter
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_ImageRecordSegments2JSONConverter
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImageRecordSegments2JSONConverter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
