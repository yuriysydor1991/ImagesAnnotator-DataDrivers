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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H

#include <memory>
#include <string>
#include <vector>

#include "IRecord.h"

namespace ImagesAnnotatorDataDrivers011
{

/**
 * @brief The particular image rectangular annotation record class.
 *
 * A single named rectangle drawn over an image. Its coordinates are stored
 * in the image own pixel space, so they stay valid regardless of any preview
 * scale applied by the presentation layer.
 *
 * Current file is a target for the library header installation.
 */
class ImageRecordRect : virtual public IRecord,
                        std::enable_shared_from_this<ImageRecordRect>
{
 public:
  using ImageRecordRectPtr = std::shared_ptr<ImageRecordRect>;
  using ImageRecordRectSet = std::vector<ImageRecordRectPtr>;

  virtual ~ImageRecordRect() = default;
  ImageRecordRect();
  ImageRecordRect(const std::string& nname, const int& nx, const int& ny,
                  const int& nwidth, const int& nheight);

  /// @brief The annotation (class) name of the rectangle
  std::string name;

  int x{0};
  int y{0};
  int width{0};
  int height{0};

  /**
   * @brief Creates a detached deep copy of the current rectangle.
   *
   * @return Returns a new instance holding the same values.
   */
  ImageRecordRectPtr duplicate_shared() const;

  /**
   * @brief Compares two rectangle sets value wise.
   *
   * @return Returns true when both sets hold equal rectangles in equal order.
   */
  static bool equal(const ImageRecordRectSet& l, const ImageRecordRectSet& r);

  /**
   * @brief Compares two rectangles value wise.
   *
   * @return Returns true when both rectangles hold the same name and geometry.
   */
  static bool equal(const ImageRecordRectPtr& l, const ImageRecordRectPtr& r);
};

using ImageRecordRectPtr = ImageRecordRect::ImageRecordRectPtr;
using ImageRecordRectSet = ImageRecordRect::ImageRecordRectSet;

}  // namespace ImagesAnnotatorDataDrivers011

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H
