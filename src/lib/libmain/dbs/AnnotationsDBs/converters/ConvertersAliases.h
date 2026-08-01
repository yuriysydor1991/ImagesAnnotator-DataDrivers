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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H

#include <nlohmann/json.hpp>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "project-global-decls.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The converters classes types holder.
 */
class ConvertersAliases : public virtual AnnotationsDBTypes
{
 public:
  // The record type aliases are inherited from the AnnotationsDBTypes virtual
  // base. Re-declaring them here would make every name ambiguous inside the
  // converters, which derive from both holders.

  inline static const auto& fdb =
      AnnotationsJSONSerializator::annotation_db_field_name;
  inline static const auto& fpath =
      AnnotationsJSONSerializator::annotations_db_path_field_name;
  inline static const auto& fann =
      AnnotationsJSONSerializator::annotations_field_name;
  inline static const auto& frel_path =
      AnnotationsJSONSerializator::annotation_recerod_rel_path_field;
  inline static const auto& fname =
      AnnotationsJSONSerializator::annotation_rect_name;
  inline static const auto& frect =
      AnnotationsJSONSerializator::annotation_rect;
  inline static const auto& fx = AnnotationsJSONSerializator::annotation_rect_x;
  inline static const auto& fy = AnnotationsJSONSerializator::annotation_rect_y;
  inline static const auto& fwidth =
      AnnotationsJSONSerializator::annotation_rect_width;
  inline static const auto& fheight =
      AnnotationsJSONSerializator::annotation_rect_height;
  inline static const auto& fannIScale =
      AnnotationsJSONSerializator::annotation_image_scale_factor;
  inline static const auto& fversion =
      AnnotationsJSONSerializator::file_version_field_name;

  inline static const auto& fiwidth =
      AnnotationsJSONSerializator::annotation_image_width;
  inline static const auto& fiheight =
      AnnotationsJSONSerializator::annotation_image_height;

  virtual ~ConvertersAliases() = default;
  ConvertersAliases() = default;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H
