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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IDBMERGER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IDBMERGER_ABSTRACT_CLASS_H

#include <memory>

#include "ImageRecord.h"

namespace ImagesAnnotatorDataDrivers013
{

/**
 * @brief The annotations databases merger interface.
 *
 * Defines how two image records sets are combined into one. The library
 * default implementation appends only the records whose resolved path is not
 * present in the destination yet and keeps the destination sorted.
 *
 * Current file is a target for the library header installation.
 */
class IDBMerger
{
 public:
  using IDBMergerPtr = std::shared_ptr<IDBMerger>;

  virtual ~IDBMerger() = default;
  IDBMerger() = default;

  /**
   * @brief Merges the append set into the dst set in place.
   *
   * @param dst The destination records set, modified by the call.
   * @param append The records set to merge into the destination.
   */
  virtual void merge(ImageRecordsSet& dst, const ImageRecordsSet& append) = 0;
};

using IDBMergerPtr = IDBMerger::IDBMergerPtr;

}  // namespace ImagesAnnotatorDataDrivers013

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IDBMERGER_ABSTRACT_CLASS_H
