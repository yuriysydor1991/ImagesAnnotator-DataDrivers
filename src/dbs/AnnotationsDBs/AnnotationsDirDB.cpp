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

#include "src/dbs/AnnotationsDBs/AnnotationsDirDB.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "src/dbs/AnnotationsDBs/LibraryImpl.h"
#include "src/dbs/AnnotationsDBs/mergers/DefaultDBMerger.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

AnnotationsDirDB::AnnotationsDirDB() : impl{LibraryImpl::create()}
{
  update_current_last_saved_no_check();
}

AnnotationsDirDB::AnnotationsDirDBPtr AnnotationsDirDB::create()
{
  return std::make_shared<AnnotationsDirDB>();
}

bool AnnotationsDirDB::load_db(const std::string& fpath)
{
  assert(!fpath.empty());
  assert(impl != nullptr);

  if (fpath.empty()) {
    LOGE("No filepath is given");
    return false;
  }

  current_db_path = fpath;

  if (!impl->load(fpath, irdb)) {
    return false;
  }

  update_current_last_saved();

  return true;
}

void AnnotationsDirDB::update_current_last_saved()
{
  update_current_last_saved_no_check();

  if (changed()) {
    LOGE("Duplicated images record set is not equal to original");
  }
}

void AnnotationsDirDB::update_current_last_saved_no_check()
{
  last_saved_db = ImageRecord::duplicate(irdb);

  assert(ImageRecord::equal(last_saved_db, irdb));
}

AnnotationsDirDB::ImageRecordsSet& AnnotationsDirDB::get_images_db()
{
  return irdb;
}

void AnnotationsDirDB::add_images_db(const ImageRecordsSet& andb)
{
  if (db_merger != nullptr) {
    db_merger->merge(irdb, andb);
    return;
  }

  auto merger = merger::DefaultDBMerger::create();

  assert(merger != nullptr);

  merger->merge(irdb, andb);
}

bool AnnotationsDirDB::store_db() { return store_db(current_db_path); }

bool AnnotationsDirDB::store_db(const std::string& fpath)
{
  assert(impl != nullptr);

  current_db_path = fpath;

  const bool sret = impl->store(fpath, get_images_db());

  if (sret) {
    update_current_last_saved();
  } else {
    LOGE("Failure during records saving");
  }

  return sret;
}

std::string AnnotationsDirDB::get_db_path() { return current_db_path; }

AnnotationsDirDB::AnnotationsList AnnotationsDirDB::get_available_annotations()
{
  AnnotationsList alist;

  for (const auto& ir : irdb) {
    assert(ir != nullptr);

    for (const auto& rect : ir->rects) {
      assert(rect != nullptr);
      alist.insert(rect->name);
    }
  }

  return alist;
}

bool AnnotationsDirDB::changed()
{
  return !ImageRecord::equal(last_saved_db, irdb);
}

bool AnnotationsDirDB::delete_image_record(const std::string& irFullPath)
{
  auto nend =
      std::remove_if(irdb.begin(), irdb.end(), [irFullPath](ImageRecordPtr ir) {
        return ir != nullptr && ir->get_full_path() == irFullPath;
      });

  irdb.erase(nend, irdb.end());

  return true;
}

void AnnotationsDirDB::set_app_info(const std::string& name,
                                    const std::string& version)
{
  assert(impl != nullptr);

  impl->set_app_info(name, version);
}

std::string AnnotationsDirDB::get_app_name() const
{
  assert(impl != nullptr);

  return impl->get_app_name();
}

std::string AnnotationsDirDB::get_app_version() const
{
  assert(impl != nullptr);

  return impl->get_app_version();
}

void AnnotationsDirDB::set_records_factory(IRecordsFactoryPtr factory)
{
  assert(impl != nullptr);

  impl->set_records_factory(factory);
}

void AnnotationsDirDB::set_db_merger(IDBMergerPtr merger)
{
  db_merger = merger;
}

void AnnotationsDirDB::set_library_impl(LibraryImplPtr newImpl)
{
  if (newImpl == nullptr) {
    LOGE("No valid project file driver given, keeping the current one");
    return;
  }

  impl = newImpl;
}

}  // namespace iannotator::dbs::annotations
