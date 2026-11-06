/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDSEGMENT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDSEGMENT_CLASS_H

#include <cstddef>
#include <memory>
#include <string>

#include "ImageRecordRect.h"

namespace ImagesAnnotatorDataDrivers015
{

/**
 * @brief The particular image segmentation annotation record class: the
 * polygon outlining an object inside its rectangle.
 *
 * The rectangle is the bounding box of the polygon, and every point of the
 * polygon lies in the 0..1 space of that rectangle - the x is the share of the
 * rectangle width from its left edge, the y the share of its height from its
 * top edge - so a moved or a resized rectangle carries its polygon along.
 *
 * Current file is a target for the library header installation.
 */
class ImageRecordSegment : public ImageRecordRect
{
 public:
  using ImageRecordSegmentPtr = std::shared_ptr<ImageRecordSegment>;

  /// @brief The least count of the points a polygon is made of
  inline static constexpr const std::size_t min_points = 3U;

  virtual ~ImageRecordSegment() = default;
  ImageRecordSegment() = default;
  ImageRecordSegment(const std::string& nname, const int& nx, const int& ny,
                     const int& nwidth, const int& nheight,
                     const Points& npoints);

  /// @brief The polygon points in the 0..1 space of the rectangle
  Points points;

  /**
   * @brief Appends a point of the image pixel space to the polygon and makes
   * the rectangle the bounding box of the whole polygon. A point within a
   * pixel of the last one adds nothing.
   */
  void add_point(const double& px, const double& py);

  virtual ImageRecordRectPtr duplicate_shared() const override;

  /**
   * @brief Reports the polygon in the image pixel space.
   *
   * @return Returns the polygon points, or the rectangle corners while the
   * polygon has less than min_points of them.
   */
  virtual Points outline() const override;

 protected:
  virtual bool equals(const ImageRecordRect& r) const override;

 private:
  /// @brief The polygon points in the image pixel space
  Points image_points() const;
  /// @brief Makes the rectangle the bounding box of the given points of the
  /// image pixel space and keeps them in its 0..1 space
  void fit(const Points& image);
};

using ImageRecordSegmentPtr = ImageRecordSegment::ImageRecordSegmentPtr;

}  // namespace ImagesAnnotatorDataDrivers015

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORDSEGMENT_CLASS_H
