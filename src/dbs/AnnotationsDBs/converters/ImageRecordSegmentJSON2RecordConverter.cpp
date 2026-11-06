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

#include "src/dbs/AnnotationsDBs/converters/ImageRecordSegmentJSON2RecordConverter.h"

#include <exception>
#include <nlohmann/json.hpp>
#include <string>

#include "ImageRecordSegment.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::converters
{

ImageRecordSegmentJSON2RecordConverter::ImageRecordRectPtr
ImageRecordSegmentJSON2RecordConverter::convert(const nlohmann::json& rect)
{
  if (!rect.contains(fsegment)) {
    return ImageRecordRectJSON2RecordConverter::convert(rect);
  }

  try {
    const auto& rd = rect.at(frect);

    return efactory->create_image_segment_record(
        rect.at(fname).get<std::string>(), rd.at(fx).get<int>(),
        rd.at(fy).get<int>(), rd.at(fwidth).get<int>(),
        rd.at(fheight).get<int>(),
        rect.at(fsegment).get<ImageRecordSegment::Points>());
  }
  catch (const std::exception& e) {
    LOGE("Exception during image segment creation: " << e.what());
  }

  return nullptr;
}

}  // namespace iannotator::dbs::annotations::converters
