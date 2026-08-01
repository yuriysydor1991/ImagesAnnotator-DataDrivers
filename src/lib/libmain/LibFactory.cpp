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

#include "src/lib/libmain/LibFactory.h"

#include <cassert>
#include <memory>

#include "src/lib/libmain/LibMain.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/mergers/DefaultDBMerger.h"
#include "src/lib/libmain/records/RecordsFactory.h"

namespace lib0impl
{

LibFactory::ILibPtr LibFactory::create_default_lib()
{
  return LibMain::create();
}

LibFactory::LibraryContextPtr LibFactory::create_default_context()
{
  return std::make_shared<LibraryContext>();
}

LibFactory::ILibPtr LibFactory::create_appropriate_lib(
    [[maybe_unused]] LibraryContextPtr ctx)
{
  assert(ctx != nullptr);

  return create_default_lib();
}

LibFactory::IAnnotationsDBPtr LibFactory::create_annotations_db()
{
  return std::make_shared<iannotator::dbs::annotations::AnnotationsDirDB>();
}

LibFactory::IAnnotationsDBPtr LibFactory::create_annotations_db(
    LibraryContextPtr ctx)
{
  auto db = std::make_shared<iannotator::dbs::annotations::AnnotationsDirDB>();

  assert(db != nullptr);

  if (ctx == nullptr) {
    return db;
  }

  db->set_app_info(ctx->app_name, ctx->app_version);

  if (ctx->records_factory != nullptr) {
    db->set_records_factory(ctx->records_factory);
  }

  if (ctx->merger != nullptr) {
    db->set_db_merger(ctx->merger);
  }

  return db;
}

LibFactory::IRecordsFactoryPtr LibFactory::create_records_factory()
{
  return iannotator::records::RecordsFactory::create();
}

LibFactory::IDBMergerPtr LibFactory::create_db_merger()
{
  return iannotator::dbs::annotations::merger::DefaultDBMerger::create();
}

LibFactoryPtr LibFactory::create_factory()
{
  return std::make_shared<LibFactory>();
}

}  // namespace lib0impl
