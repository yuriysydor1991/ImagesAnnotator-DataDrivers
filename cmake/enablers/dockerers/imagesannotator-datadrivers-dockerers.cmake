cmake_minimum_required(VERSION 3.13)

option(
  IADD_ENABLE_DOCKER
  "Set to ON to enable the dockers run"
  OFF
)

if (NOT IADD_ENABLE_DOCKER)
    return()
endif()

message(STATUS "Docker runner target enabled")

find_program(DOCKER_EXEC docker REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/dockerers")

include(imagesannotator-datadrivers-docker-variables-declare)
include(imagesannotator-datadrivers-docker-Jenkins-pipeline-runner-target)
