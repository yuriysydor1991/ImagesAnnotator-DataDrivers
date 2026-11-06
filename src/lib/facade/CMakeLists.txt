cmake_minimum_required(VERSION 3.13)

target_sources(
  ${IADD_LIBRARY_NAME}
  PRIVATE
    IADataDriversFacade.cpp
    IADataDriversContext.cpp
)

install(DIRECTORY public/ DESTINATION include/${IADD_LIBRARY_NAME})

add_subdirectory(tests)
