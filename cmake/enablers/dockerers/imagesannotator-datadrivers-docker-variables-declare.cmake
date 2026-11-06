cmake_minimum_required(VERSION 3.13)

string(TOLOWER ${IADD_LIBRARY_NAME} IADD_LIBRARY_NAME_lower)

set(
  IADD_DOCKER_HOST_ADDRESS
  "127.0.0.1"
  CACHE STRING "The docker service machine address"
)

set(
  IADD_DOCKER_HOST_PORT
  "2375"
  CACHE STRING "The docker service machine port"
)

set(
  IADD_DOCKER_HOST_STR
  "tcp://${IADD_DOCKER_HOST_ADDRESS}:${IADD_DOCKER_HOST_PORT}"
  CACHE STRING "The docker service machine full address"
)

set(
  IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_NAME
  "jenkins/jenkins"
  CACHE STRING 
  "The Dockerfile source environment name (FROM ubuntu, debian, jenkins etc.)"
)

set(
  IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_VERSION
  "lts"
  CACHE STRING 
  "The Dockerfile source environment version"
)

set(
  IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_STRING
  "${IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_NAME}:${IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_VERSION}"
  CACHE STRING 
  "The Dockerfile source environment full string (e.g. ubuntu:22.04)"
)

set(
  PROJECT_BINARY_PATH
  ${PROJECT_BINARY_DIR}/src/${IADD_LIBRARY_NAME}
)

message(STATUS "Docker executable: ${DOCKER_EXEC}")
message(STATUS "Dockerfile configure source full path: ${DOCKERFILE_SRC}")
message(STATUS "Dockerfile destination name: ${DOCKERFILE_DST}")
message(STATUS "Docker host address and port: ${IADD_DOCKER_HOST_STR}")
message(STATUS "Project current branch name: ${PROJECT_CURRENT_BRANCH_NAME}")
message(STATUS "Project binary filepath: ${PROJECT_BINARY_PATH}")
message(STATUS "Docker source environment base image string: ${IADD_DOCKERFILE_SINGLE_RUN_BASE_IMAGE_STRING}")
