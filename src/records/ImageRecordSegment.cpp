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

#include "ImageRecordSegment.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>

namespace ImagesAnnotatorDataDrivers015
{

namespace
{
double share(const double& offset, const int& size)
{
  return size == 0 ? 0.0 : offset / size;
}

int floored(const double& value) { return static_cast<int>(std::floor(value)); }

int ceiled(const double& value) { return static_cast<int>(std::ceil(value)); }
}  // namespace

ImageRecordSegment::ImageRecordSegment(const std::string& nname, const int& nx,
                                       const int& ny, const int& nwidth,
                                       const int& nheight,
                                       const Points& npoints)
    : ImageRecordRect{nname, nx, ny, nwidth, nheight}, points{npoints}
{
}

void ImageRecordSegment::add_point(const double& px, const double& py)
{
  Points image = image_points();

  if (!image.empty()) {
    const auto& [lx, ly] = image.back();

    if (std::abs(px - lx) < 1.0 && std::abs(py - ly) < 1.0) {
      return;
    }
  }

  image.emplace_back(px, py);

  fit(image);
}

ImageRecordRectPtr ImageRecordSegment::duplicate_shared() const
{
  return std::make_shared<ImageRecordSegment>(*this);
}

ImageRecordRect::Points ImageRecordSegment::outline() const
{
  return points.size() < min_points ? ImageRecordRect::outline()
                                    : image_points();
}

bool ImageRecordSegment::equals(const ImageRecordRect& r) const
{
  const auto* segment = dynamic_cast<const ImageRecordSegment*>(&r);

  return segment != nullptr && ImageRecordRect::equals(r) &&
         points == segment->points;
}

ImageRecordRect::Points ImageRecordSegment::image_points() const
{
  Points image;

  image.reserve(points.size() + 1U);

  std::transform(points.cbegin(), points.cend(), std::back_inserter(image),
                 [this](const Point& p) -> Point {
                   const auto& [px, py] = p;
                   return {x + px * width, y + py * height};
                 });

  return image;
}

void ImageRecordSegment::fit(const Points& image)
{
  const auto [left, right] = std::minmax_element(
      image.cbegin(), image.cend(), [](const Point& l, const Point& r) {
        return std::get<0>(l) < std::get<0>(r);
      });
  const auto [top, bottom] = std::minmax_element(
      image.cbegin(), image.cend(), [](const Point& l, const Point& r) {
        return std::get<1>(l) < std::get<1>(r);
      });

  x = floored(std::get<0>(*left));
  y = floored(std::get<1>(*top));
  width = ceiled(std::get<0>(*right)) - x;
  height = ceiled(std::get<1>(*bottom)) - y;

  points.clear();

  for (const auto& [px, py] : image) {
    points.emplace_back(share(px - x, width), share(py - y, height));
  }
}

}  // namespace ImagesAnnotatorDataDrivers015
