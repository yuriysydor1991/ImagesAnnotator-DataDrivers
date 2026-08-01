cmake_minimum_required(VERSION 3.13)

include(template-project-compile-options)
add_compile_options(${EXTRA_COMPILE_OPTIONS})

configure_file(
  project-global-decls.h.in
  ${CMAKE_BINARY_DIR}/project-global-decls.h
)

set(GENERAL_MOCKS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tests/mocks")

# Per-component source lists shared with the test executables. The unit tests
# compile the components straight from the tree (against the mocks under
# src/tests/mocks) instead of linking the produced shared library, so they need
# to name the very same translation units. Declared here, before any
# subdirectory is added, so that every test tree below can see them.
set(IADD_RECORDS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/records/ImageRecord.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/records/ImageRecordRect.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/records/RecordsFactory.cpp
)

set(IADD_HELPERS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/helpers/SortHelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helpers/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helpers/UrlHelper.cpp
)

set(IADD_CONVERTERS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/dbs/AnnotationsDBs/converters
)

set(IADD_ANNOTATIONS_DBS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/dbs/AnnotationsDBs
)

set(IADD_PUBLIC_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/facade/public
)

# The lib subdirectory declares the library target, so it comes first: every
# implementation component below registers its sources against that target.
add_subdirectory(lib)

add_subdirectory(records)
add_subdirectory(helpers)
add_subdirectory(dbs)

add_subdirectory(log)
