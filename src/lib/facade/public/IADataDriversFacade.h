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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSFACADE_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSFACADE_CLASS_H

#include <memory>
#include <string>

#include "IADataDriversContext.h"
#include "IADataDriversLib.h"
#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

/**
 * @brief The logging subsystem interface every project built from the
 * cpp-app-template shares. The definition compiled into this library is token
 * identical to the one of the project which uses it, so this installed header
 * only forward declares it and never collides with the consumer own
 * logger::ILogger definition.
 */
namespace logger
{
class ILogger;
}  // namespace logger

namespace ImagesAnnotatorDataDrivers013
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
 * namespace iadd = ImagesAnnotatorDataDrivers013;
 *
 * auto db = iadd::IADataDriversFacade::open_annotations_db("project.json");
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
class IADataDriversFacade
{
 public:
  virtual ~IADataDriversFacade() = default;
  IADataDriversFacade() = default;

  /**
   * @brief Factory method to create a library context pre-filled with the
   * compile time defaults.
   *
   * @return Returns a new IADataDriversContext class instance.
   */
  static IADataDriversContextPtr create_library_context();

  /**
   * @brief Creates the default library implementation.
   *
   * @return Returns the default library implementation. Currently returns
   * the LibMain class instance.
   */
  static IADataDriversLibPtr create_default_lib();

  /**
   * @brief Creates and returns the appropriate IADataDriversLib instance.
   *
   * @param ctx Filled IADataDriversContext to check for an appropriate
   * IADataDriversLib instance.
   *
   * @return Returns a new IADataDriversLib descendant or a nullptr in case of
   * any error.
   */
  static IADataDriversLibPtr create_library(IADataDriversContextPtr ctx);

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
  static IAnnotationsDBPtr create_annotations_db(IADataDriversContextPtr ctx);

  /**
   * @brief Creates an annotations database and immediately loads the given
   * project file into it.
   *
   * @param fpath The project file path to read: the imgajz1 one of the
   * .imgajz1 file name extension, the JSON one of any other.
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

  /**
   * @brief Hands the logger instance of the project which uses this library
   * over to the library logging subsystem, so the whole binary logs into the
   * very same logger instead of the library keeping a silent own one.
   *
   * Call it before the first use of the library, usually right after the own
   * logging initialization. Any logger::ILogger implementation is accepted: a
   * project built from the cpp-app-template shares its default logger by
   * passing the LOG_REAL_LOGGER() value.
   *
   * @param realLogger The logger instance to adopt. A nullptr is ignored and
   * the previously adopted or the library own instance stays.
   */
  static void accept_real_logger(
      const std::shared_ptr<logger::ILogger>& realLogger);
};

}  // namespace ImagesAnnotatorDataDrivers013

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSFACADE_CLASS_H
