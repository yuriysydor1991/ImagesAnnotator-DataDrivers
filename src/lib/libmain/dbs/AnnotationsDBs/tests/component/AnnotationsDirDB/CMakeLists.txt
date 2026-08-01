cmake_minimum_required(VERSION 3.13)

# The component test drives the produced shared library through its installable
# public headers only - no mocks and no library internal header is reachable
# from here. The logger comes with the library, so this executable is nothing
# but the test translation unit itself.
add_executable(
  CTEST_AnnotationsDirDB
  CTEST_AnnotationsDirDB.cpp
)

target_compile_options(
  CTEST_AnnotationsDirDB
  PRIVATE -DCTEST_DATA_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/ctest_data"
)

target_include_directories(
  CTEST_AnnotationsDirDB
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
  PRIVATE ${ANNOTATIONS_DBS_CTESTS_INCLUDE_DIR}
)

target_link_libraries(
  CTEST_AnnotationsDirDB
  GTest::gtest_main GTest::gmock ${PROJECT_LIBRARY_NAME}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_AnnotationsDirDB
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
