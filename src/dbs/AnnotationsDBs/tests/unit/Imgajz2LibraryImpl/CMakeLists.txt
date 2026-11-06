cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_Imgajz2LibraryImpl
  UTEST_Imgajz2LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/Imgajz1LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/Imgajz2LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/LibraryImpl.cpp
  ${IADD_ANNOTATIONS_DBS_SRC_DIR}/AnnotationsJSONSerializator.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecords2JSONConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/JSON2ImageRecordsConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordRectJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordSegmentJSON2RecordConverter.cpp
  ${IADD_CONVERTERS_SRC_DIR}/ImageRecordSegments2JSONConverter.cpp
  ${IADD_RECORDS_SRC}
  ${IADD_ZLIB_SRC}
)

target_include_directories(
  UTEST_Imgajz2LibraryImpl
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_Imgajz2LibraryImpl
  GTest::gtest_main GTest::gmock nlohmann_json::nlohmann_json ZLIB::ZLIB
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_Imgajz2LibraryImpl
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
