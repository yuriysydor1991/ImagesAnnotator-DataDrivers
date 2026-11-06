cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(
  ${IADD_LIBRARY_NAME}
  SHARED
)

# The very target name the installed CMake package exports, so a project links
# the same one whether it finds the installed library or takes it in by the
# FetchContent.
add_library(${IADD_LIBRARY_NAME}::${IADD_LIBRARY_NAME} ALIAS ${IADD_LIBRARY_NAME})

# The forwarding headers of the installed paths, so the prefixed includes below
# work against the sources too. The file(GENERATE) rewrites only the changed
# ones, a reconfigure recompiles nothing.
set(IADD_BUILD_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

file(
  GLOB_RECURSE IADD_PUBLIC_HEADERS
  RELATIVE "${IADD_PUBLIC_INCLUDE_DIR}"
  CONFIGURE_DEPENDS "${IADD_PUBLIC_INCLUDE_DIR}/*.h"
)

foreach(publicHeader IN LISTS IADD_PUBLIC_HEADERS)
  file(
    GENERATE
    OUTPUT "${IADD_BUILD_INCLUDE_DIR}/${IADD_LIBRARY_NAME}/${publicHeader}"
    CONTENT "#include \"${IADD_PUBLIC_INCLUDE_DIR}/${publicHeader}\"\n"
  )
endforeach()

target_include_directories(
  ${IADD_LIBRARY_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${PROJECT_BINARY_DIR}
  PUBLIC
    # Both the plain include root and the versioned sub-directory are exported,
    # of the build tree and of the installation alike, so a consumer may write
    # either
    #   #include <ImagesAnnotatorDataDrivers-0.15/IADataDriversFacade.h>  (recommended)
    # or the short
    #   #include <IADataDriversFacade.h>
    # The prefixed form is the safe one: header names such as ImageRecord.h or
    # IRecord.h are generic enough to collide in a busy include path.
    $<BUILD_INTERFACE:${IADD_BUILD_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${IADD_PUBLIC_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/${IADD_LIBRARY_NAME}>
)

set_target_properties(
  ${IADD_LIBRARY_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS ${IADD_LIBRARY_NAME}
  LIBRARY
    DESTINATION lib
    COMPONENT Runtime
    NAMELINK_COMPONENT Development
)

add_subdirectory(facade)
add_subdirectory(libmain)
add_subdirectory(tests)

include(lib-cmake-module-gen)

