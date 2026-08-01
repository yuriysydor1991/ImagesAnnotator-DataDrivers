cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(
  ${PROJECT_LIBRARY_NAME}
  SHARED
)

target_include_directories(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/facade/public>
    # Both the plain include root and the versioned sub-directory are exported,
    # so a consumer may write either
    #   #include <ImagesAnnotatorDataDrivers-0/LibraryFacade.h>   (recommended)
    # or the short
    #   #include <LibraryFacade.h>
    # The prefixed form is the safe one: header names such as ImageRecord.h or
    # IRecord.h are generic enough to collide in a busy include path.
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/${PROJECT_LIBRARY_NAME}>
)

set_target_properties(
  ${PROJECT_LIBRARY_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS ${PROJECT_LIBRARY_NAME}
  LIBRARY
    DESTINATION lib
    COMPONENT Runtime
    NAMELINK_COMPONENT Development
)

# Per-component source lists shared with the test executables. The unit tests
# compile the components straight from the tree (against the mocks under
# src/tests/mocks) instead of linking the produced shared library, so they need
# to name the very same translation units.
set(IADD_RECORDS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/records/ImageRecord.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/records/ImageRecordRect.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/records/RecordsFactory.cpp
)

set(IADD_HELPERS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/helpers/SortHelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/helpers/ImageRecordUrlAndPathHelper.cpp
)

set(IADD_CURL_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/CURL/CURLController.cpp
)

set(IADD_CONVERTERS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/dbs/AnnotationsDBs/converters
)

set(IADD_ANNOTATIONS_DBS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/libmain/dbs/AnnotationsDBs
)

set(IADD_PUBLIC_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/facade/public
)

add_subdirectory(facade)
add_subdirectory(libmain)

include(lib-cmake-module-gen)

