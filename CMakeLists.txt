cmake_minimum_required(VERSION 3.13)

project(
  ImagesAnnotatorDataDrivers
  VERSION 0.12.0
  DESCRIPTION "The ImagesAnnotator project JSON annotations database library"
  HOMEPAGE_URL "http://www.kytok.org.ua/"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/template-project-bootstrap.cmake)

message(STATUS "PROJECT_NAME: ImagesAnnotatorDataDrivers")

add_subdirectory(src)
add_subdirectory(doc)

include(template-project-feature-enabler-post)
