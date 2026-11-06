cmake_minimum_required(VERSION 3.13)

if(IADD_IS_TOP_LEVEL)
  include(imagesannotator-datadrivers-dockerers)
endif()

# The mandatory dependencies were made available by the pre-src enabler pass,
# here they only get linked against the now existing library target.
include(imagesannotator-datadrivers-nlohmann-json-linker)
include(imagesannotator-datadrivers-libcurl-linker)
include(imagesannotator-datadrivers-zlib-linker)
