cmake_minimum_required(VERSION 3.13)

add_library(
  TemplateProjectDefaultLoggerObj OBJECT
  DefaultLogger.cpp
)

target_include_directories(
  TemplateProjectDefaultLoggerObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

# The objects end up inside a shared library, so they must be relocatable and
# they must stay hidden: the logger is an implementation detail and its unique
# bound inline statics (the default log file name above all) would otherwise be
# unified with the identically named ones of the application or of any other
# library built from this template, handing this library log file name to the
# whole process.
set_target_properties(
  TemplateProjectDefaultLoggerObj
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  CXX_VISIBILITY_PRESET hidden
)

# The library code logs through the src/log macros, which resolve to this
# logger. Without this link the produced shared object would ship unresolved
# default_logger::DefaultLogger symbols and every consumer would fail to link
# against it. PRIVATE because the logger is an implementation detail: no
# installable public header mentions it.
target_link_libraries(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE TemplateProjectDefaultLoggerObj
)

add_subdirectory(real-default-logger)
add_subdirectory(tests)
