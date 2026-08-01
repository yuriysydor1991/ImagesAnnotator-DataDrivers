cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_RecordsFactory
  UTEST_RecordsFactory.cpp
  ${IADD_RECORDS_SRC}
)

target_include_directories(
  UTEST_RecordsFactory
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_RecordsFactory
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_RecordsFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
