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

#include "ImageRecordRect.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <unordered_set>

#include "IRecord.h"
#include "src/log/log.h"

namespace ImagesAnnotatorDataDrivers015
{

ImageRecordRect::ImageRecordRect() {}

ImageRecordRect::ImageRecordRect(const std::string& nname, const int& nx,
                                 const int& ny, const int& nwidth,
                                 const int& nheight)
    : name{nname}, x{nx}, y{ny}, width{nwidth}, height{nheight}
{
}

ImageRecordRectPtr ImageRecordRect::duplicate_shared() const
{
  return std::make_shared<ImageRecordRect>(*this);
}

bool ImageRecordRect::equal(const ImageRecordRectSet& l,
                            const ImageRecordRectSet& r)
{
  if (&l == &r) {
    LOGT("The same object given as parameters");
    return true;
  }

  const bool rt = std::equal(
      l.begin(), l.end(), r.begin(), r.end(),
      [](const ImageRecordRectPtr& lirr, const ImageRecordRectPtr& rirr) {
        return equal(lirr, rirr);
      });

  LOGT("Equality value: " << rt);

  return rt;
}

ImageRecordRect::Points ImageRecordRect::outline() const
{
  const double left = x;
  const double top = y;
  const double right = x + width;
  const double bottom = y + height;

  return {{left, top}, {right, top}, {right, bottom}, {left, bottom}};
}

bool ImageRecordRect::equal(const ImageRecordRectPtr& l,
                            const ImageRecordRectPtr& r)
{
  if (l.get() == r.get()) {
    LOGT("The same object given");
    return true;
  }

  // either side compares the data of its own type, so an annotation never
  // equals one of another type
  const bool rt = l->equals(*r) && r->equals(*l);

  LOGT("Equality value for " << l->name << " = " << rt);

  return rt;
}

bool ImageRecordRect::equals(const ImageRecordRect& r) const
{
  LOGT("Comparing: " << name << " ?= " << r.name);
  LOGT("         : " << x << " ?= " << r.x);
  LOGT("         : " << y << " ?= " << r.y);
  LOGT("         : " << width << " ?= " << r.width);
  LOGT("         : " << height << " ?= " << r.height);

  return name == r.name && x == r.x && y == r.y && width == r.width &&
         height == r.height;
}

}  // namespace ImagesAnnotatorDataDrivers015
