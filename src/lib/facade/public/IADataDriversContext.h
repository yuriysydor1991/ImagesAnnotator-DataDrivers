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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H

#include <memory>
#include <string>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

namespace ImagesAnnotatorDataDrivers013
{

/**
 * @brief The library context class designed to pass data in and out of
 * the library underlying implementation.
 *
 * A default constructed context is already usable: app_name and app_version
 * are seeded with the compile time defaults, which keep the produced project
 * files readable by the ImagesAnnotator application. Those defaults are
 * controlled by the ANNOTATIONS_DB_ROOT_APP_NAME and
 * ANNOTATIONS_DB_APP_VERSION CMake cache variables.
 *
 * Current file is a target for the library header installation.
 */
class IADataDriversContext
{
 public:
  using IADataDriversContextPtr = std::shared_ptr<IADataDriversContext>;

  virtual ~IADataDriversContext() = default;

  /**
   * @brief Builds a context filled with the library compile time defaults.
   */
  IADataDriversContext();

  /// @brief In: the project file IADataDriversLib::libcall is to open, the
  /// imgajz1 one of the .imgajz1 file name extension, the JSON one of any
  /// other. When left empty the libcall only creates an empty database
  /// instance.
  std::string db_path;

  /// @brief In: the JSON root element name of the project files. Defaults to
  /// the compile time ANNOTATIONS_DB_ROOT_APP_NAME value.
  std::string app_name;

  /// @brief In: the version string stored under the JSON root element.
  /// Defaults to the compile time ANNOTATIONS_DB_APP_VERSION value.
  std::string app_version;

  /// @brief In: an optional custom records factory the deserialization is to
  /// build the records with. The library default one is used when left empty.
  IRecordsFactoryPtr records_factory;

  /// @brief In: an optional custom merger IAnnotationsDB::add_images_db is to
  /// use. The library default one is used when left empty.
  IDBMergerPtr merger;

  /// @brief Out: the database instance produced by the last
  /// IADataDriversLib::libcall.
  IAnnotationsDBPtr db;

  static IADataDriversContextPtr create();
};

using IADataDriversContextPtr = IADataDriversContext::IADataDriversContextPtr;

}  // namespace ImagesAnnotatorDataDrivers013

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H
