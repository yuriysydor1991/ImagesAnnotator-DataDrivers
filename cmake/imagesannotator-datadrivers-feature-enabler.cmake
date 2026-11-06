cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/tools")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/enablers")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/compile-options")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/compile-options/sanitizers")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/enablers/dockerers")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/enablers/packagers")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/enablers/compression")

include(imagesannotator-datadrivers-enabler-function)
include(imagesannotator-datadrivers-git-enabler)

# Mandatory library dependencies. They are made available here, before src/ is
# added, so that both the library target and the test executables declared
# inside it can be linked against them.
include(imagesannotator-datadrivers-nlohmann-json-enabler)
include(imagesannotator-datadrivers-libcurl-enabler)
include(imagesannotator-datadrivers-zlib-enabler)

# The tests, the tools and the packages of the top level build only: the same
# named targets and the CPack of an including project stay its own.
if(IADD_IS_TOP_LEVEL)
  include(imagesannotator-datadrivers-GTest-enabler)
  include(imagesannotator-datadrivers-clang-format-target)

  # packagers
  include(imagesannotator-datadrivers-deb-enabler)
  include(imagesannotator-datadrivers-freebsd-pkg-enabler)
  include(imagesannotator-datadrivers-wix-enabler)
  include(imagesannotator-datadrivers-rpm-enabler)

  # compile options
  include(imagesannotator-datadrivers-sanitizers)

  # analyzers
  include(imagesannotator-datadrivers-cppcheck-target)
  include(imagesannotator-datadrivers-clang-tidy-target)
endif()
