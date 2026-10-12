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

#include "src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <memory>
#include <string>

#include "src/log/log.h"
#include "src/zlib/ZlibController.h"

namespace iannotator::dbs::annotations
{

Imgajz1LibraryImpl::Imgajz1LibraryImplPtr Imgajz1LibraryImpl::create()
{
  return std::make_shared<Imgajz1LibraryImpl>();
}

Imgajz1LibraryImpl::Imgajz1LibraryImplPtr Imgajz1LibraryImpl::create(
    const LibraryImpl& like)
{
  auto impl = create();

  impl->set_app_info(like.get_app_name(), like.get_app_version());
  impl->set_records_factory(like.get_records_factory());

  return impl;
}

bool Imgajz1LibraryImpl::is_imgajz1_path(const std::string& fpath)
{
  std::string extension = std::filesystem::path{fpath}.extension().string();

  std::transform(
      extension.begin(), extension.end(), extension.begin(),
      [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });

  return extension == file_extension;
}

bool Imgajz1LibraryImpl::read_text(const std::string& fpath, std::string& text)
{
  LOGT("Trying to decompress the imgajz1 file: " << fpath);

  const iadd0zlibi::ZlibController::buffer decompressed =
      iadd0zlibi::ZlibController::create()->uncompress_from_file(fpath);

  // the controller logs the reason of the file unable to be read
  if (decompressed.empty()) {
    LOGE("No JSON project text decompressed out of the file: " << fpath);
    return false;
  }

  text.assign(decompressed.cbegin(), decompressed.cend());

  return true;
}

bool Imgajz1LibraryImpl::write_text(const std::string& fpath,
                                    const std::string& text)
{
  LOGT("Trying to compress the JSON project text into the file: " << fpath);

  return iadd0zlibi::ZlibController::create()->compress_to_file(
      fpath, {text.cbegin(), text.cend()});
}

}  // namespace iannotator::dbs::annotations
