cmake_minimum_required(VERSION 3.13)

# The nlohmann_json library is made available (system wide or through the
# Internet via FetchContent) by the imagesannotator-datadrivers-nlohmann-json-enabler
# module and linked against the library by its
# imagesannotator-datadrivers-nlohmann-json-linker companion. The zlib of the imgajz1
# project files comes the same way, through the imagesannotator-datadrivers-zlib-enabler
# and the imagesannotator-datadrivers-zlib-linker modules.

target_sources(
  ${IADD_LIBRARY_NAME}
  PRIVATE
    AnnotationsDirDB.cpp
    AnnotationsJSONSerializator.cpp
    Imgajz1LibraryImpl.cpp
    Imgajz2LibraryImpl.cpp
    LibraryImpl.cpp
)

add_subdirectory(converters)
add_subdirectory(mergers)

add_subdirectory(tests)
