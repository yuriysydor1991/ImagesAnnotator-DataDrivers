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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IIMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IIMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H

#include <memory>
#include <set>
#include <string>

#include "IProvider.h"
#include "ImageRecord.h"

namespace ImagesAnnotatorDataDrivers011
{

/**
 * @brief The read and query interface over an opened annotations database.
 *
 * Implement this interface in the consuming project whenever an alternative
 * in-memory records source has to be fed to the code that only reads the
 * database. The library own IAnnotationsDB descendants extend it with the
 * loading and storing abilities.
 *
 * Current file is a target for the library header installation.
 */
class IImagesPathsDBProvider : virtual public IProvider
{
 public:
  using IImagesPathsDBProviderPtr = std::shared_ptr<IImagesPathsDBProvider>;

  /// @brief The sorted set of the distinct annotation (class) names
  using AnnotationsList = std::set<std::string>;

  virtual ~IImagesPathsDBProvider() = default;
  IImagesPathsDBProvider() = default;

  /**
   * @brief Gives a modifiable access to the in-memory image records.
   *
   * @return Returns the reference to the held image records set.
   */
  virtual ImageRecordsSet& get_images_db() = 0;

  /**
   * @brief Reports the filesystem path of the currently opened project file.
   *
   * @return Returns the project JSON db file path or an empty string.
   */
  virtual std::string get_db_path() = 0;

  /**
   * @brief Collects every distinct annotation name used across the records.
   *
   * @return Returns the sorted set of the annotation names.
   */
  virtual AnnotationsList get_available_annotations() = 0;

  /**
   * @brief Tells whether the in-memory records differ from the stored ones.
   *
   * @return Returns true when there are unsaved modifications.
   */
  virtual bool changed() = 0;

  /**
   * @brief Removes the record whose full path equals the given one.
   *
   * @param irFullPath The ImageRecord::get_full_path() value to erase.
   *
   * @return Returns true on the successfully performed removal attempt.
   */
  virtual bool delete_image_record(const std::string& irFullPath) = 0;
};

using IImagesPathsDBProviderPtr =
    IImagesPathsDBProvider::IImagesPathsDBProviderPtr;

}  // namespace ImagesAnnotatorDataDrivers011

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IIMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
