cmake_minimum_required(VERSION 3.15)

option(
  IADD_ENABLE_CLANG_TIDY
  "Set to ON if enable the clang-tidy code check target."
  OFF
)

if(NOT IADD_ENABLE_CLANG_TIDY)
  return()
endif()

find_program(CLTIDY_EXEC clang-tidy REQUIRED)

message(STATUS "clang-tidy: ${CLTIDY_EXEC}")

foreach(DIR IN LISTS CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
  if(EXISTS ${DIR})
    list(APPEND implicitSysIncludes ${DIR})
  else()
    message(STATUS "The system include directory does not exist (clang-tidy): ${DIR}")
  endif()
endforeach()

list(TRANSFORM implicitSysIncludes PREPEND "--extra-arg=-I")

set(
  CTCOMMAND
  ${CLTIDY_EXEC} 
    --config-file "${PROJECT_SOURCE_DIR}/misc/.clang-tidy"
    -p "${PROJECT_BINARY_DIR}"
    --extra-arg=-Wno-unused-command-line-argument
    ${implicitSysIncludes}
)

message(STATUS "clang-tidy command: ${CTCOMMAND}")

set(CMAKE_CXX_CLANG_TIDY ${CTCOMMAND})