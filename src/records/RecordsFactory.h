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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_RECORDSFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_RECORDSFACTORY_CLASS_H

#include <memory>
#include <string>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"

namespace iannotator::records
{

/**
 * @brief The default library data records factory implementation.
 *
 * Creates the plain library records. A consuming project may derive from the
 * public ImagesAnnotatorDataDrivers012::IRecordsFactory (or from the current
 * class) in order to obtain its own enriched records out of the JSON
 * deserialization.
 */
class RecordsFactory : public ImagesAnnotatorDataDrivers012::IRecordsFactory
{
 public:
  using RecordsFactoryPtr = std::shared_ptr<RecordsFactory>;
  using ImageRecord = ImagesAnnotatorDataDrivers012::ImageRecord;
  using ImageRecordPtr = ImagesAnnotatorDataDrivers012::ImageRecordPtr;
  using ImageRecordRect = ImagesAnnotatorDataDrivers012::ImageRecordRect;
  using ImageRecordRectPtr = ImagesAnnotatorDataDrivers012::ImageRecordRectPtr;

  virtual ~RecordsFactory() = default;
  RecordsFactory() = default;

  /**
   * @brief Creates a new image record.
   *
   * @param nirpath The image path relative to nabsPath.
   * @param nabsPath The root directory or web page URL of the image.
   *
   * @return Returns the newly created image record.
   */
  virtual ImageRecordPtr create_image_record(
      const std::string& nirpath = "",
      const std::string& nabsPath = "") override;

  /**
   * @brief Creates an empty rectangle with an auto generated name.
   *
   * @param ir The record the rectangle is going to be appended to. When it is
   * a nullptr the default_name_record name is used, otherwise the name is
   * numbered after the already present rectangles count.
   *
   * @return Returns the newly created rectangle.
   */
  virtual ImageRecordRectPtr create_image_rect_record(
      const ImageRecordPtr& ir = nullptr) override;

  /**
   * @brief Creates a fully defined rectangle.
   *
   * @return Returns the newly created rectangle.
   */
  virtual ImageRecordRectPtr create_image_rect_record(
      const std::string& nname, const int& nx, const int& ny, const int& nwidth,
      const int& nheight) override;

  static RecordsFactoryPtr create();
};

using RecordsFactoryPtr = RecordsFactory::RecordsFactoryPtr;

}  // namespace iannotator::records

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_RECORDSFACTORY_CLASS_H
