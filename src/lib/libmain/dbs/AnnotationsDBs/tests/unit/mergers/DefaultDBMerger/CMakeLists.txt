cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_DefaultDBMerger
  UTEST_DefaultDBMerger.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/mergers/DefaultDBMerger.cpp
  ${IADD_HELPERS_SRC}
  ${IADD_CURL_SRC}
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_DefaultDBMerger
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_DefaultDBMerger
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json CURL::libcurl
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_DefaultDBMerger
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
