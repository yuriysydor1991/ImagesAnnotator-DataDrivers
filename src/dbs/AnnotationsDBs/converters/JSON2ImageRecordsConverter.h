/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H

#include <nlohmann/json.hpp>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "project-global-decls.h"
#include "src/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The annotator annotations JSON data to ImageRecordsSet converter.
 */
class JSON2ImageRecordsConverter : public virtual AnnotationsDBTypes,
                                   virtual public ConvertersAliases
{
 public:
  virtual ~JSON2ImageRecordsConverter() = default;
  JSON2ImageRecordsConverter();
  /**
   * @param factory The factory the records are built with.
   * @param rectConverter The converter of the annotations of a record. A
   * nullptr makes the one of the rectangles alone.
   */
  explicit JSON2ImageRecordsConverter(
      IRecordsFactoryPtr factory,
      ImageRecordRectJSON2RecordConverterPtr rectConverter = nullptr);

  virtual ImageRecordsSet fetch_records(const nlohmann::json& allAJSon);

 protected:
  virtual bool convert_folder(const nlohmann::json& afolder,
                              ImageRecordsSet& rset);
  virtual ImageRecordPtr convert_annotation(const nlohmann::json& fan,
                                            const std::string& absdirpath);

 private:
  IRecordsFactoryPtr efactory;
  ImageRecordRectJSON2RecordConverterPtr rectConv;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
