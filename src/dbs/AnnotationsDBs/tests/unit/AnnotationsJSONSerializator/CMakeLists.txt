cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_AnnotationsJSONSerializator
  UTEST_AnnotationsJSONSerializator.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/AnnotationsJSONSerializator.cpp
)

target_include_directories(
  UTEST_AnnotationsJSONSerializator
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_AnnotationsJSONSerializator
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_AnnotationsJSONSerializator
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
