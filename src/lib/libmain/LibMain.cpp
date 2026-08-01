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

#include "src/lib/libmain/LibMain.h"

#include <cassert>
#include <memory>

#include "src/lib/libmain/LibFactory.h"
#include "src/log/log.h"

namespace lib0impl
{

LibMain::LibMain()
{
  static bool inited{false};

  if (!inited) {
    LOG_INIT_DEFAULTS();
    inited = true;
  }
}

bool LibMain::libcall(LibraryContextPtr ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid library context pointer provided");
    return false;
  }

  LOGT("Creating the annotations database instance");

  auto libFactory = LibFactory::create_factory();

  assert(libFactory != nullptr);

  auto db = libFactory->create_annotations_db(ctx);

  assert(db != nullptr);

  if (db == nullptr) {
    LOGE("Fail to create the annotations database instance");
    return false;
  }

  if (!ctx->db_path.empty()) {
    LOGI("Loading the annotations db: " << ctx->db_path);

    if (!db->load_db(ctx->db_path)) {
      LOGE("Fail to load the annotations db: " << ctx->db_path);
      return false;
    }
  } else {
    LOGT("No db path provided, leaving the created database empty");
  }

  ctx->db = db;

  LOGI("The annotations database instance is ready");

  return true;
}

LibMainPtr LibMain::create() { return std::make_shared<LibMain>(); }

}  // namespace lib0impl
