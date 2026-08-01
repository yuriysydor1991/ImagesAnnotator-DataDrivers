cmake_minimum_required(VERSION 3.13)

# The implementation of the public data record classes declared in the
# installable facade/public headers, plus the default records factory the
# JSON deserialization builds them with.

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE
    ImageRecord.cpp
    ImageRecordRect.cpp
    RecordsFactory.cpp
)

add_subdirectory(tests)
