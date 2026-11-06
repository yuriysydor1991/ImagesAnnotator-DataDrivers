cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImageRecordSegment
  UTEST_ImageRecordSegment.cpp
  ${PROJECT_SOURCE_DIR}/src/records/ImageRecordRect.cpp
  ${PROJECT_SOURCE_DIR}/src/records/ImageRecordSegment.cpp
)

target_include_directories(
  UTEST_ImageRecordSegment
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_ImageRecordSegment
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImageRecordSegment
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
