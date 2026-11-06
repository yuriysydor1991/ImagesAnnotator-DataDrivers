cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_IADataDriversFacade
  UTEST_IADataDriversFacade.cpp
  ${PROJECT_SOURCE_DIR}/src/lib/facade/IADataDriversFacade.cpp
  ${PROJECT_SOURCE_DIR}/src/log/default-logger/DefaultLogger.cpp
  ${PROJECT_SOURCE_DIR}/src/log/default-logger/real-default-logger/RealDefaultLogger.cpp
)

target_include_directories(
  UTEST_IADataDriversFacade
  PRIVATE ${GENERAL_MOCKS_ROOT}/IADataDriversContext
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibMain
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${FACADE_MOCK_DIR}/LibFactory
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${IADD_PUBLIC_INCLUDE_DIR}
)

target_link_libraries(
  UTEST_IADataDriversFacade
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_IADataDriversFacade
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
