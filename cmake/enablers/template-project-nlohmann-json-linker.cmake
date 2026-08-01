cmake_minimum_required(VERSION 3.13)

# Companion of the template-project-nlohmann-json-enabler module: links the
# already made available nlohmann json library against the project library
# target, which only exists once src/ has been added.

if (NOT ENABLE_NLOHMANN_JSON)
  return()
endif()

# PRIVATE on purpose: no installable public header of the library exposes a
# nlohmann type, so the dependency must not leak into the exported interface.
# A PUBLIC (or plain signature) link would put the FetchContent provided
# nlohmann_json target into INTERFACE_LINK_LIBRARIES and break the
# install(EXPORT) of the library.
target_link_libraries(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE nlohmann_json::nlohmann_json
)
