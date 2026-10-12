cmake_minimum_required(VERSION 3.13)

# The zlib compression controller of the cpp-app-template appZlib branch, as
# the darknetxx adapted it for its gzip compressed dxxwjz files: the in memory
# and the gzip file compression and the gzip files streams, which read and
# write the files a chunk at a time. The imgajz1 project files are the gzip
# compressed JSON documents it writes and reads.

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE
    GzipFileStreams.cpp
    ZlibController.cpp
)

add_subdirectory(tests)
