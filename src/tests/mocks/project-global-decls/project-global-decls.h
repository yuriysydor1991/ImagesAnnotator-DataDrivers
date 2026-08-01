#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_GLOBAL_DECLS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_GLOBAL_DECLS_H

#include <string>

/**
 * @brief The unit tests stand-in for the CMake generated
 * project-global-decls.h header.
 *
 * Unit tests compile single translation units straight from the source tree,
 * without the configured build directory on their include path, so they pick
 * this fixed copy up instead. The values are frozen on purpose: a test that
 * asserts on the produced JSON must not start failing the moment the project
 * version is bumped.
 */
namespace project_decls
{

/// @brief Global project' library name
const std::string PROJECT_NAME = "ImagesAnnotatorDataDrivers-0";
/// @brief Global project' build version
const std::string PROJECT_BUILD_VERSION = "0.11.0";
/// @brief Global project' configure git commit hash
const std::string PROJECT_BUILD_COMMIT =
    "0000000000000000000000000000000000000000";
/// @brief Global project' configure date
const std::string PROJECT_CONFIGURE_DATE = "2025-01-01 00:00:00";

/// @brief The JSON root element name of the annotations project files
const std::string ANNOTATIONS_DB_ROOT_APP_NAME = "ImagesAnnotator";
/// @brief The version string written under the JSON root element
const std::string ANNOTATIONS_DB_APP_VERSION = "0.11.0";

}  // namespace project_decls

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_GLOBAL_DECLS_H
