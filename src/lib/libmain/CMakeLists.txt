cmake_minimum_required(VERSION 3.13)

target_sources(
  ${IADD_LIBRARY_NAME}
  PRIVATE
    LibMain.cpp
    LibFactory.cpp
)

add_subdirectory(tests)
