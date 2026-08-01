/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H

#include <nlohmann/json.hpp>

#include "ImageRecord.h"
#include "project-global-decls.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db JSON data serializator.
 */
class AnnotationsJSONSerializator
{
 public:
  virtual ~AnnotationsJSONSerializator() = default;
  AnnotationsJSONSerializator() = default;

  /**
   * @brief Constructs the serializator bound to the given application
   * identity instead of the compile time default one.
   *
   * @param appName The JSON root element name to expect inside the files.
   * @param appVersion The version string to compare the files ones against.
   */
  AnnotationsJSONSerializator(const std::string& appName,
                              const std::string& appVersion);

  virtual bool serialize(const nlohmann::json& json);

  /// @brief The compile time JSON root element name default
  inline static const std::string default_root_app_name =
      project_decls::ANNOTATIONS_DB_ROOT_APP_NAME;
  /// @brief The compile time files version default
  inline static const std::string default_app_version =
      project_decls::ANNOTATIONS_DB_APP_VERSION;

  /// @brief The JSON root element name currently in use
  std::string root_app_name{default_root_app_name};
  /// @brief The application version currently expected inside the files
  std::string app_version{default_app_version};

  inline static constexpr const char* const annotations_field_name =
      "annotations";
  inline static constexpr const char* const file_version_field_name = "version";
  inline static constexpr const char* const annotation_db_field_name = "db";
  inline static constexpr const char* const annotations_db_path_field_name =
      "path";

  inline static constexpr const char* const annotation_recerod_rel_path_field =
      "rel_path";

  inline static constexpr const char* const annotation_rect_name = "name";
  inline static constexpr const char* const annotation_rect = "rect";
  inline static constexpr const char* const annotation_rect_x = "x";
  inline static constexpr const char* const annotation_rect_y = "y";
  inline static constexpr const char* const annotation_rect_width = "width";
  inline static constexpr const char* const annotation_rect_height = "height";
  inline static constexpr const char* const annotation_image_scale_factor =
      "scale_factor";

  inline static constexpr const char* const annotation_image_width =
      annotation_rect_width;
  inline static constexpr const char* const annotation_image_height =
      annotation_rect_height;

 protected:
  virtual bool serialize_app_info(const nlohmann::json& json);
  virtual bool serialize_annotations_records(const nlohmann::json& json);
  virtual bool serialize_annotations_record(const nlohmann::json& arecords);
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H
