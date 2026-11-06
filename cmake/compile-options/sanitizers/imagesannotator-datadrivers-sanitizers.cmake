cmake_minimum_required(VERSION 3.13)

option(
  IADD_ENABLE_SANITIZERS
  "Set to ON to enable the sanitizers compile options"
  OFF
)

option(
  IADD_ENABLE_SANITIZERS_THREADS
  "Set to ON to enable the sanitizers for the threads compile options"
  OFF
)

if (NOT (IADD_ENABLE_SANITIZERS OR IADD_ENABLE_SANITIZERS_THREADS))
  return()
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/sanitizers")

if (IADD_ENABLE_SANITIZERS)
  include(imagesannotator-datadrivers-sanitizers-leak-and-address)
endif()

if (IADD_ENABLE_SANITIZERS_THREADS)
  include(imagesannotator-datadrivers-sanitizers-threads)
endif()
