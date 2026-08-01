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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H

#include <nlohmann/json.hpp>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IImagesPathsDBProvider.h"
#include "IRecord.h"
#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "project-global-decls.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations db types aliases.
 */
class AnnotationsDBTypes
{
 public:
  using IRecord = ImagesAnnotatorDataDrivers011::IRecord;
  using ImageRecord = ImagesAnnotatorDataDrivers011::ImageRecord;
  using ImageRecordPtr = ImagesAnnotatorDataDrivers011::ImageRecordPtr;
  using ImageRecordsSet = ImagesAnnotatorDataDrivers011::ImageRecordsSet;
  using ImageRecordRect = ImagesAnnotatorDataDrivers011::ImageRecordRect;
  using ImageRecordRectPtr = ImagesAnnotatorDataDrivers011::ImageRecordRectPtr;
  using ImageRecordRectSet = ImagesAnnotatorDataDrivers011::ImageRecordRectSet;
  using IImagesPathsDBProvider =
      ImagesAnnotatorDataDrivers011::IImagesPathsDBProvider;
  using IImagesPathsDBProviderPtr =
      ImagesAnnotatorDataDrivers011::IImagesPathsDBProviderPtr;
  using IAnnotationsDB = ImagesAnnotatorDataDrivers011::IAnnotationsDB;
  using IAnnotationsDBPtr = ImagesAnnotatorDataDrivers011::IAnnotationsDBPtr;
  using IRecordsFactory = ImagesAnnotatorDataDrivers011::IRecordsFactory;
  using IRecordsFactoryPtr = ImagesAnnotatorDataDrivers011::IRecordsFactoryPtr;
  using IDBMerger = ImagesAnnotatorDataDrivers011::IDBMerger;
  using IDBMergerPtr = ImagesAnnotatorDataDrivers011::IDBMergerPtr;

  AnnotationsDBTypes() = default;
  virtual ~AnnotationsDBTypes() = default;
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H
