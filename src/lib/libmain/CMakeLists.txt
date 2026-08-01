cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE
    LibMain.cpp
    LibFactory.cpp
)

add_subdirectory(records)
add_subdirectory(helpers)
add_subdirectory(CURL)
add_subdirectory(dbs)

add_subdirectory(tests)
