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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "IRecord.h"

namespace ImagesAnnotatorDataDrivers015
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

  /// @brief A point of an annotation outline: its x and its y
  using Point = std::tuple<double, double>;
  using Points = std::vector<Point>;

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
   * @return Returns a new instance of the same type holding the same values.
   */
  virtual ImageRecordRectPtr duplicate_shared() const;

  /**
   * @brief Reports the outline of the annotation in the image pixel space.
   *
   * @return Returns the four corners, clockwise from the top left one.
   */
  virtual Points outline() const;

  /**
   * @brief Compares two rectangle sets value wise.
   *
   * @return Returns true when both sets hold equal rectangles in equal order.
   */
  static bool equal(const ImageRecordRectSet& l, const ImageRecordRectSet& r);

  /**
   * @brief Compares two rectangles value wise.
   *
   * @return Returns true when both annotations are of the same type and
   * hold the same data.
   */
  static bool equal(const ImageRecordRectPtr& l, const ImageRecordRectPtr& r);

 protected:
  /// @brief Compares the data of the own type with the given annotation ones
  virtual bool equals(const ImageRecordRect& r) const;
};

using ImageRecordRectPtr = ImageRecordRect::ImageRecordRectPtr;
using ImageRecordRectSet = ImageRecordRect::ImageRecordRectSet;

}  // namespace ImagesAnnotatorDataDrivers015

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDRECT_CLASS_H
