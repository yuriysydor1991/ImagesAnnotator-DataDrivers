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
  using IRecord = ImagesAnnotatorDataDrivers012::IRecord;
  using ImageRecord = ImagesAnnotatorDataDrivers012::ImageRecord;
  using ImageRecordPtr = ImagesAnnotatorDataDrivers012::ImageRecordPtr;
  using ImageRecordsSet = ImagesAnnotatorDataDrivers012::ImageRecordsSet;
  using ImageRecordRect = ImagesAnnotatorDataDrivers012::ImageRecordRect;
  using ImageRecordRectPtr = ImagesAnnotatorDataDrivers012::ImageRecordRectPtr;
  using ImageRecordRectSet = ImagesAnnotatorDataDrivers012::ImageRecordRectSet;
  using IImagesPathsDBProvider =
      ImagesAnnotatorDataDrivers012::IImagesPathsDBProvider;
  using IImagesPathsDBProviderPtr =
      ImagesAnnotatorDataDrivers012::IImagesPathsDBProviderPtr;
  using IAnnotationsDB = ImagesAnnotatorDataDrivers012::IAnnotationsDB;
  using IAnnotationsDBPtr = ImagesAnnotatorDataDrivers012::IAnnotationsDBPtr;
  using IRecordsFactory = ImagesAnnotatorDataDrivers012::IRecordsFactory;
  using IRecordsFactoryPtr = ImagesAnnotatorDataDrivers012::IRecordsFactoryPtr;
  using IDBMerger = ImagesAnnotatorDataDrivers012::IDBMerger;
  using IDBMergerPtr = ImagesAnnotatorDataDrivers012::IDBMergerPtr;

  AnnotationsDBTypes() = default;
  virtual ~AnnotationsDBTypes() = default;
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H
