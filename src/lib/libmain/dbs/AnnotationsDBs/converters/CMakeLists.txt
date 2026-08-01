cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE
    JSON2ImageRecordsConverter.cpp
    ImageRecords2JSONConverter.cpp
    ImageRecordRectJSON2RecordConverter.cpp
    ImageRecordJSON2RecordConverter.cpp
)
