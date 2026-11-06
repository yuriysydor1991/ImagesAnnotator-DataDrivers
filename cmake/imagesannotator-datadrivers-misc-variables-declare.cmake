cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ON by default: the public API namespace carries the major and the minor
# version (ImagesAnnotatorDataDrivers015), so the installable names track the
# same pair. Two minor versions then install completely side by side - their
# binaries, their header directories and their CMake packages all differ.
option(
  IADD_LIB_INCLUDE_MINOR_IN_NAME
  "Append .<minor> to the installable library name (binary, headers subdir, CMake package dir)"
  ON
)

option(
  IADD_LIB_INCLUDE_MICRO_IN_NAME
  "Append .<micro> after the minor segment to the installable library name. Implies IADD_LIB_INCLUDE_MINOR_IN_NAME."
  OFF
)

set(
  IADD_LIB_NAME_SUFFIX ""
  CACHE STRING
  "Optional trailing suffix appended to the installable library name (e.g. '-dev')."
)

set(_lib_name "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}")

if(IADD_LIB_INCLUDE_MICRO_IN_NAME AND NOT IADD_LIB_INCLUDE_MINOR_IN_NAME)
  message(WARNING
    "IADD_LIB_INCLUDE_MICRO_IN_NAME=ON requires IADD_LIB_INCLUDE_MINOR_IN_NAME=ON - "
    "enabling IADD_LIB_INCLUDE_MINOR_IN_NAME implicitly."
  )
  set(IADD_LIB_INCLUDE_MINOR_IN_NAME ON CACHE BOOL "" FORCE)
endif()

if(IADD_LIB_INCLUDE_MINOR_IN_NAME)
  set(_lib_name "${_lib_name}.${PROJECT_VERSION_MINOR}")
endif()

if(IADD_LIB_INCLUDE_MICRO_IN_NAME)
  set(_lib_name "${_lib_name}.${PROJECT_VERSION_PATCH}")
endif()

if(IADD_LIB_NAME_SUFFIX)
  set(_lib_name "${_lib_name}${IADD_LIB_NAME_SUFFIX}")
endif()

set(
  IADD_LIBRARY_NAME "${_lib_name}"
  CACHE STRING "Project main library name and target"
  FORCE
)

unset(_lib_name)

message(STATUS "IADD_LIBRARY_NAME: ${IADD_LIBRARY_NAME}")

set(
  IADD_MAINTAINER "Your Name"
  CACHE STRING "Project maintainer name (used for the DEB package)"
)

set(
  IADD_MAINTAINER_EMAIL "yourname@your.email.org"
  CACHE STRING "Project maintainer email (used for the DEB package)"
)

set(
  IADD_ANNOTATIONS_DB_ROOT_APP_NAME "ImagesAnnotator"
  CACHE STRING
  "The JSON root element name of the annotations project files. Keep the default to stay compatible with the ImagesAnnotator application project files."
)

set(
  IADD_ANNOTATIONS_DB_APP_VERSION "${PROJECT_VERSION}"
  CACHE STRING
  "The version string written under the annotations project files JSON root element"
)

set(
  IADD_MAX_LOG_LEVEL "3"
  CACHE STRING
  "Sets the maximum severity of the compiled log messages. Error=0. Trace=5. By default Info=3"
)

set(
  IADD_DEFAULT_LOG_FILE_PATH ""
  CACHE STRING
  "Sets the default log file path"
)

option(
  IADD_ENABLE_LOGS_MICROSECONDS_TIME
  "Set to OFF to disable the microseconds fields included into the log messages"
  ON
)

string(TIMESTAMP PROJECT_CONFIGURE_DATE "%Y-%m-%d %H:%M:%S")
