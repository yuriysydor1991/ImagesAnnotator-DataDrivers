cmake_minimum_required(VERSION 3.13)

# The annotations project database is a JSON document, so unlike in the
# original template this dependency is mandatory for the library to build.
#
# This module only makes the dependency AVAILABLE. It runs before the library
# target exists, so that the unit and component test executables declared
# inside src/ may already link against it. The linking against the library
# target itself is performed later by the companion
# template-project-nlohmann-json-linker module.
option(
  ENABLE_NLOHMANN_JSON
  "Set to ON to enable the nlohmann json library (by using system wide available or through Internet). Required by the annotations database."
  ON
)

if (NOT ENABLE_NLOHMANN_JSON)
  return()
endif()

set(TEMPLATE_APP_NLOHMANN_GIT "https://github.com/nlohmann/json.git" CACHE STRING "The Nlohmann JSON library git source repository")
set(TEMPLATE_APP_NLOHMANN_GIT_TAG "v3.11.3" CACHE STRING "The Nlohmann JSON project git repository tag of interest")

template_project_default_3rdparty_enabler(
  NAME nlohmann_json
  GIT_REPOSITORY ${TEMPLATE_APP_NLOHMANN_GIT}
  GIT_TAG ${TEMPLATE_APP_NLOHMANN_GIT_TAG}
)
