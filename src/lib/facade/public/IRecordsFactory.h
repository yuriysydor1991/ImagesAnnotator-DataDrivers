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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IRECORDSFACTORY_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IRECORDSFACTORY_ABSTRACT_CLASS_H

#include <memory>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"

namespace ImagesAnnotatorDataDrivers014
{

/**
 * @brief The library data records factory interface.
 *
 * The JSON to records converters build every record through this factory, so
 * a consuming project may substitute its own descendant in order to obtain
 * enriched record subclasses out of the deserialization.
 *
 * Current file is a target for the library header installation.
 */
class IRecordsFactory
{
 public:
  using IRecordsFactoryPtr = std::shared_ptr<IRecordsFactory>;

  /// @brief The name given to a rectangle created without an owning record
  inline static const std::string default_name_record = "rectangle_default";

  /// @brief The prefix of the auto generated per-image rectangle names
  inline static const std::string default_numbered_name_record_prefix =
      "rectangle_default_";

  virtual ~IRecordsFactory() = default;
  IRecordsFactory() = default;

  /**
   * @brief Creates a new image record.
   *
   * @param nirpath The image path relative to nabsPath.
   * @param nabsPath The root directory or web page URL of the image.
   *
   * @return Returns the newly created image record.
   */
  virtual ImageRecordPtr create_image_record(
      const std::string& nirpath = "", const std::string& nabsPath = "") = 0;

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
      const ImageRecordPtr& ir = nullptr) = 0;

  /**
   * @brief Creates a fully defined rectangle.
   *
   * @return Returns the newly created rectangle.
   */
  virtual ImageRecordRectPtr create_image_rect_record(const std::string& nname,
                                                      const int& nx,
                                                      const int& ny,
                                                      const int& nwidth,
                                                      const int& nheight) = 0;
};

using IRecordsFactoryPtr = IRecordsFactory::IRecordsFactoryPtr;

}  // namespace ImagesAnnotatorDataDrivers014

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IRECORDSFACTORY_ABSTRACT_CLASS_H
