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

#include "src/lib/libmain/records/RecordsFactory.h"

#include <memory>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/log/log.h"

namespace iannotator::records
{

RecordsFactory::ImageRecordPtr RecordsFactory::create_image_record(
    const std::string& nirpath, const std::string& nabsPath)
{
  return std::make_shared<ImageRecord>(nirpath, nabsPath);
}

RecordsFactory::ImageRecordRectPtr RecordsFactory::create_image_rect_record(
    const ImageRecordPtr& ir)
{
  auto rt = std::make_shared<ImageRecordRect>();

  if (ir == nullptr) {
    rt->name = default_name_record;
  } else {
    rt->name =
        default_numbered_name_record_prefix + std::to_string(ir->rects.size());
  }

  return rt;
}

RecordsFactory::ImageRecordRectPtr RecordsFactory::create_image_rect_record(
    const std::string& nname, const int& nx, const int& ny, const int& nwidth,
    const int& nheight)
{
  return std::make_shared<ImageRecordRect>(nname, nx, ny, nwidth, nheight);
}

RecordsFactoryPtr RecordsFactory::create()
{
  return std::make_shared<RecordsFactory>();
}

}  // namespace iannotator::records
