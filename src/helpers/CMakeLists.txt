cmake_minimum_required(VERSION 3.13)

# UrlHelper is the only reason the library links libcurl: it resolves the web
# hosted image records through the libcurl URL (CURLU) API. The dependency is
# made available by the imagesannotator-datadrivers-libcurl-enabler module and linked by
# its imagesannotator-datadrivers-libcurl-linker companion.

target_sources(
  ${IADD_LIBRARY_NAME}
  PRIVATE
    SortHelper.cpp
    ImageRecordUrlAndPathHelper.cpp
    UrlHelper.cpp
)
