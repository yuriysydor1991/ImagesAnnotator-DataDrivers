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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYFACADE_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYFACADE_CLASS_H

#include <memory>
#include <string>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "ILib.h"
#include "IRecordsFactory.h"
#include "LibraryContext.h"

namespace ImagesAnnotatorDataDrivers011
{

/**
 * @brief The facade interface class of the library.
 *
 * This is the only entry point a consuming project needs: every concrete
 * implementation class of the library stays hidden behind the abstract
 * interfaces returned from here.
 *
 * A typical read - modify - write session looks like this:
 *
 * @code
 * namespace iadd = ImagesAnnotatorDataDrivers011;
 *
 * auto db = iadd::LibraryFacade::open_annotations_db("project.json");
 *
 * if (db == nullptr) {
 *   return false;
 * }
 *
 * for (const auto& ir : db->get_images_db()) {
 *   // inspect or edit ir->rects here
 * }
 *
 * return !db->changed() || db->store_db();
 * @endcode
 *
 * Current file is a target for the library header installation.
 */
class LibraryFacade
{
 public:
  virtual ~LibraryFacade() = default;
  LibraryFacade() = default;

  /**
   * @brief Factory method to create a library context pre-filled with the
   * compile time defaults.
   *
   * @return Returns a new LibraryContext class instance.
   */
  static LibraryContextPtr create_library_context();

  /**
   * @brief Creates the default library implementation.
   *
   * @return Returns the default library implementation. Currently returns
   * the LibMain class instance.
   */
  static ILibPtr create_default_lib();

  /**
   * @brief Creates and returns the appropriate ILib instance.
   *
   * @param ctx Filled LibraryContext to check for an appropriate ILib instance.
   *
   * @return Returns a new ILib descendant or a nullptr in case of any error.
   */
  static ILibPtr create_library(LibraryContextPtr ctx);

  /**
   * @brief Creates an empty annotations database carrying the library
   * compile time application name and version defaults.
   *
   * @return Returns a new IAnnotationsDB descendant instance.
   */
  static IAnnotationsDBPtr create_annotations_db();

  /**
   * @brief Creates an empty annotations database configured by the given
   * context: the application identity, the records factory and the merger
   * are all taken from it when set.
   *
   * @param ctx The context to configure the new database with. A nullptr is
   * treated as an empty default context.
   *
   * @return Returns a new IAnnotationsDB descendant instance.
   */
  static IAnnotationsDBPtr create_annotations_db(LibraryContextPtr ctx);

  /**
   * @brief Creates an annotations database and immediately loads the given
   * project JSON file into it.
   *
   * @param fpath The project JSON db file path to read.
   *
   * @return Returns the loaded database or a nullptr when the file could not
   * be read or parsed.
   */
  static IAnnotationsDBPtr open_annotations_db(const std::string& fpath);

  /**
   * @brief Creates the library default data records factory.
   *
   * @return Returns a new IRecordsFactory descendant instance.
   */
  static IRecordsFactoryPtr create_records_factory();

  /**
   * @brief Creates the library default annotations databases merger.
   *
   * @return Returns a new IDBMerger descendant instance.
   */
  static IDBMergerPtr create_db_merger();

  /**
   * @brief Reports the version of the library binary being used.
   *
   * @return Returns the library build version string.
   */
  static std::string library_version();

  /**
   * @brief Reports the JSON root element name new databases default to.
   *
   * @return Returns the compile time ANNOTATIONS_DB_ROOT_APP_NAME value.
   */
  static std::string default_db_root_app_name();

  /**
   * @brief Reports the version string new databases write by default.
   *
   * @return Returns the compile time ANNOTATIONS_DB_APP_VERSION value.
   */
  static std::string default_db_app_version();
};

}  // namespace ImagesAnnotatorDataDrivers011

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYFACADE_CLASS_H
