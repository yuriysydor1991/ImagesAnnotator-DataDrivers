cmake_minimum_required(VERSION 3.13)

option(
  IADD_ENABLE_UNIT_TESTS 
  "Set to ON value if unit tests build and run should be available"
  OFF
)

option(
  IADD_ENABLE_COMPONENT_TESTS 
  "Set to ON value if the component tests build and run should be available"
  OFF
)

option(
  IADD_GTEST_TRY_SYSTEM_PROBE 
  "Set to ON value if current project CMake files should probe the system GTest"
  ON
)

if(NOT (IADD_ENABLE_UNIT_TESTS OR IADD_ENABLE_COMPONENT_TESTS))
  return()
endif()

enable_testing()

set(IADD_GTEST_GIT "https://github.com/google/googletest.git" CACHE STRING "The GoogleTest git source repository")
set(IADD_GTEST_GIT_TAG "v1.16.0" CACHE STRING "The GoogleTest project git repository tag of interest")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

if (IADD_GTEST_TRY_SYSTEM_PROBE)
  set(DISABLE_SYSTEM_PROBE "")
else()
  set(DISABLE_SYSTEM_PROBE "DISABLE_SYSTEM_PROBE")
endif()

iadd_default_3rdparty_enabler(
    NAME GTest 
    GIT_REPOSITORY ${IADD_GTEST_GIT}
    GIT_TAG ${IADD_GTEST_GIT_TAG}
    "${DISABLE_SYSTEM_PROBE}"
)
