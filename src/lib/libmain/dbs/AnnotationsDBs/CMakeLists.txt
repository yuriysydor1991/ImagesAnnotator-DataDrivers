cmake_minimum_required(VERSION 3.13)

# The nlohmann_json library is made available (system wide or through the
# Internet via FetchContent) by the template-project-nlohmann-json-enabler
# module and linked against the library by its
# template-project-nlohmann-json-linker companion.

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE
    AnnotationsDirDB.cpp
    AnnotationsJSONSerializator.cpp
)

add_subdirectory(converters)
add_subdirectory(mergers)

add_subdirectory(tests)
