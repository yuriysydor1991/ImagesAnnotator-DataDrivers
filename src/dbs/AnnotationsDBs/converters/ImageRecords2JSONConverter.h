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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H

#include <nlohmann/json.hpp>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "project-global-decls.h"
#include "src/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/converters/ConvertersAliases.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The ImageRecordsSet to the annotations project JSON document
 * converter. The project file drivers (see the LibraryImpl) write the document
 * into the files of their formats.
 */
class ImageRecords2JSONConverter : public virtual AnnotationsDBTypes,
                                   virtual public ConvertersAliases
{
 public:
  virtual ~ImageRecords2JSONConverter() = default;
  ImageRecords2JSONConverter();

  /**
   * @brief Creates a converter stamping the given application identity.
   *
   * @param appName The JSON root element name of the produced files.
   * @param appVersion The version string stored under the root element.
   */
  ImageRecords2JSONConverter(const std::string& appName,
                             const std::string& appVersion);

  /// @brief The JSON root element name the store() call writes
  std::string app_name{AnnotationsJSONSerializator::default_root_app_name};

  /// @brief The version value stored under the JSON root element
  std::string app_version{AnnotationsJSONSerializator::default_app_version};

  /**
   * @brief Converts the records into the whole project document: the
   * application identity element and the records grouped by their folders.
   *
   * @param irs The records to convert.
   *
   * @return Returns the JSON project document of the records.
   */
  virtual nlohmann::json to_document(const ImageRecordsSet& irs);

 protected:
  virtual nlohmann::json convert(const ImageRecordsSet& irs);
  virtual nlohmann::json convert(const ImageRecordPtr& ir);
  virtual nlohmann::json convert(const ImageRecordRectSet& rects);
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H
