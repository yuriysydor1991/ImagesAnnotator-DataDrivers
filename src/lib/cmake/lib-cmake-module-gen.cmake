include(GNUInstallDirs)

install(
  TARGETS ${IADD_LIBRARY_NAME}
  EXPORT ${IADD_LIBRARY_NAME}
  LIBRARY
    DESTINATION lib
    COMPONENT Runtime
    NAMELINK_COMPONENT Development
)

set(PROJECT_MODULE_TARGETS_FILE "${IADD_LIBRARY_NAME}Targets.cmake")

# Export targets (so downstream can use find_package)
install(EXPORT ${IADD_LIBRARY_NAME}
  FILE ${PROJECT_MODULE_TARGETS_FILE}
  NAMESPACE ${IADD_LIBRARY_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${IADD_LIBRARY_NAME}
)

# Generate a ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${IADD_LIBRARY_NAME}Version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Generate a Config.cmake
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TemplateLibraryConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${IADD_LIBRARY_NAME}Config.cmake.in"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${IADD_LIBRARY_NAME}
)

install(EXPORT ${IADD_LIBRARY_NAME}
  FILE "${IADD_LIBRARY_NAME}Config.cmake"
  NAMESPACE ${IADD_LIBRARY_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${IADD_LIBRARY_NAME}
)

