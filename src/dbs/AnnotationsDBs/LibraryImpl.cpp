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

#include "src/dbs/AnnotationsDBs/LibraryImpl.h"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <exception>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.h"
#include "src/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

LibraryImplPtr LibraryImpl::create() { return std::make_shared<LibraryImpl>(); }

bool LibraryImpl::load(const std::string& fpath, ImageRecordsSet& irdb)
{
  assert(!fpath.empty());

  nlohmann::json json;

  try {
    LOGT("Trying to open the file: " << fpath);

    std::ifstream f(fpath);

    if (!f.is_open()) {
      const int cerrno = errno;
      LOGE("Can't open file: " << fpath << " because " << strerror(cerrno));
      return false;
    }

    LOGT("Trying to parse the given db json");

    json = nlohmann::json::parse(f);

    LOGT("Trying to serialize retrieved file");

    if (!serialize(json)) {
      LOGE("Fail to serialize the JSON: " << json.dump());
      return false;
    }

    LOGT("Trying to extract the records");

    irdb = load_the_irs(json);

    LOGT("Now DB contains " << irdb.size() << " records");
  }
  catch (const std::exception& e) {
    LOGE("Exception occurred: " << e.what());
    return false;
  }

  return true;
}

bool LibraryImpl::store(const std::string& fpath, const ImageRecordsSet& irdb)
{
  auto irsConverter = std::make_shared<converters::ImageRecords2JSONConverter>(
      app_name, app_version);

  LOGD("Trying to store annotations to " << fpath);

  return irsConverter->store(irdb, fpath);
}

bool LibraryImpl::serialize(const nlohmann::json& json)
{
  auto s = std::make_shared<AnnotationsJSONSerializator>(app_name, app_version);

  return s->serialize(json);
}

LibraryImpl::ImageRecordsSet LibraryImpl::load_the_irs(
    const nlohmann::json& json)
{
  LOGT("Trying to load the ImageRecords into the memory");

  static const auto& fannotations =
      AnnotationsJSONSerializator::annotations_field_name;

  assert(json.contains(fannotations));

  auto converter =
      std::make_shared<converters::JSON2ImageRecordsConverter>(records_factory);

  return converter->fetch_records(json[fannotations]);
}

void LibraryImpl::set_app_info(const std::string& name,
                               const std::string& version)
{
  app_name = name;
  app_version = version;
}

std::string LibraryImpl::get_app_name() const { return app_name; }

std::string LibraryImpl::get_app_version() const { return app_version; }

void LibraryImpl::set_records_factory(IRecordsFactoryPtr factory)
{
  records_factory = factory;
}

}  // namespace iannotator::dbs::annotations
