cmake_minimum_required(VERSION 3.13)

option(
  IADD_ENABLE_RPM
  "Set to ON to enable the RPM package creation from the project build artifacts"
  OFF
)

if(NOT IADD_ENABLE_RPM)
    return()
endif()

include(InstallRequiredSystemLibraries)

set(
  IADD_RPM_LICENSE
  "BSD-2-Clause"
  CACHE STRING
  "RPM package license tag (e.g. BSD-2-Clause, BSD-3-Clause, GPLv3, MIT, Apache-2.0)"
)

set(
  IADD_RPM_GROUP
  "Applications/System"
  CACHE STRING
  "RPM package group (e.g. Applications/System, Development/Libraries)"
)

set(
  IADD_RPM_RELEASE
  "1"
  CACHE STRING
  "RPM package release number (incremented across rebuilds of the same upstream version)"
)

set(
  IADD_RPM_PACKAGE_ARCHITECTURE
  ${CMAKE_SYSTEM_PROCESSOR}
  CACHE STRING
  "RPM package architecture tag (e.g. x86_64, aarch64, noarch)"
)

set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT ${IADD_MAINTAINER_EMAIL})
set(CPACK_PACKAGE_VENDOR "${IADD_MAINTAINER}")
set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})

set(CPACK_RPM_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_RPM_PACKAGE_LICENSE ${IADD_RPM_LICENSE})
set(CPACK_RPM_PACKAGE_GROUP ${IADD_RPM_GROUP})
set(CPACK_RPM_PACKAGE_RELEASE ${IADD_RPM_RELEASE})
set(CPACK_RPM_PACKAGE_URL ${PROJECT_HOMEPAGE_URL})
set(CPACK_RPM_PACKAGE_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_RPM_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_RPM_PACKAGE_VENDOR "${IADD_MAINTAINER}")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${IADD_RPM_PACKAGE_ARCHITECTURE})
set(CPACK_RPM_PACKAGE_REQUIRES "")
set(CPACK_RPM_COMPONENT_INSTALL ON)

# the CPACK_SYSTEM_NAME of the default file name names the system alone, so the
# destination CPU takes its place, followed by the name and the version of the
# destination system
include(imagesannotator-datadrivers-package-system)

set(
  CPACK_PACKAGE_FILE_NAME
  ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${IADD_RPM_PACKAGE_ARCHITECTURE}${IADD_PACKAGE_SYSTEM_SUFFIX}
)

include(CPack)
