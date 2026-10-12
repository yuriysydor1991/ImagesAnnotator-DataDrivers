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

#include "IADataDriversFacade.h"

#include <cassert>
#include <memory>
#include <string>

#include "project-global-decls.h"
#include "src/lib/libmain/LibFactory.h"
#include "src/log/log.h"

namespace ImagesAnnotatorDataDrivers013
{

IADataDriversContextPtr IADataDriversFacade::create_library_context()
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_default_context();
}

IADataDriversLibPtr IADataDriversFacade::create_default_lib()
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_default_lib();
}

IADataDriversLibPtr IADataDriversFacade::create_library(
    IADataDriversContextPtr ctx)
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_appropriate_lib(ctx);
}

IAnnotationsDBPtr IADataDriversFacade::create_annotations_db()
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_annotations_db();
}

IAnnotationsDBPtr IADataDriversFacade::create_annotations_db(
    IADataDriversContextPtr ctx)
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_annotations_db(ctx);
}

IAnnotationsDBPtr IADataDriversFacade::open_annotations_db(
    const std::string& fpath)
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  auto db = libFactory->create_annotations_db();

  if (db == nullptr) {
    return nullptr;
  }

  if (!db->load_db(fpath)) {
    return nullptr;
  }

  return db;
}

IRecordsFactoryPtr IADataDriversFacade::create_records_factory()
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_records_factory();
}

IDBMergerPtr IADataDriversFacade::create_db_merger()
{
  auto libFactory = lib0impl::LibFactory::create_factory();

  assert(libFactory != nullptr);

  return libFactory->create_db_merger();
}

std::string IADataDriversFacade::library_version()
{
  return project_decls::PROJECT_BUILD_VERSION;
}

std::string IADataDriversFacade::default_db_root_app_name()
{
  return project_decls::ANNOTATIONS_DB_ROOT_APP_NAME;
}

std::string IADataDriversFacade::default_db_app_version()
{
  return project_decls::ANNOTATIONS_DB_APP_VERSION;
}

void IADataDriversFacade::accept_real_logger(
    const std::shared_ptr<logger::ILogger>& realLogger)
{
  LOG_INIT_REAL_LOGGER(realLogger);
}

}  // namespace ImagesAnnotatorDataDrivers013
