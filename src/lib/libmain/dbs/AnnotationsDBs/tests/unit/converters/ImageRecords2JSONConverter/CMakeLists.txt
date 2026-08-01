cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImageRecords2JSONConverter
  UTEST_ImageRecords2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecords2JSONConverter.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_ImageRecords2JSONConverter
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_ImageRecords2JSONConverter
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImageRecords2JSONConverter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
