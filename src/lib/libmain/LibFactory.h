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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H

#include <memory>

#include "IADataDriversContext.h"
#include "IADataDriversLib.h"
#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

namespace lib0impl
{

/**
 * The library main factory to operate with the implementation.
 * Create appropriate instances in accordance of provided data
 * by the given context or by simply calling appropriate method.
 */
class LibFactory
{
 public:
  using LibFactoryPtr = std::shared_ptr<LibFactory>;
  using IADataDriversLibPtr =
      ImagesAnnotatorDataDrivers013::IADataDriversLibPtr;
  using IADataDriversContextPtr =
      ImagesAnnotatorDataDrivers013::IADataDriversContextPtr;
  using IADataDriversContext =
      ImagesAnnotatorDataDrivers013::IADataDriversContext;
  using IAnnotationsDBPtr = ImagesAnnotatorDataDrivers013::IAnnotationsDBPtr;
  using IRecordsFactoryPtr = ImagesAnnotatorDataDrivers013::IRecordsFactoryPtr;
  using IDBMergerPtr = ImagesAnnotatorDataDrivers013::IDBMergerPtr;

  virtual ~LibFactory() = default;
  LibFactory() = default;

  /**
   * @brief Creates the default library implementation. May be overriden
   * in descendants.
   *
   * @return Returns the default library implementation. Currently returns
   * the LibMain class instance.
   */
  virtual IADataDriversLibPtr create_default_lib();

  /**
   * @brief Creates the default context to use in libraries implementation
   * in order to provide all necessary data.
   *
   * @return Returns empty library context instance.
   */
  virtual IADataDriversContextPtr create_default_context();

  /**
   * @brief Creates appropriate library implementation instance
   * with accordance of provided data in the given context.
   *
   * @return Returns the appropriate library implementation if any
   * or a nullptr in case of any error. Currently returns the LibMain class
   * instance only.
   */
  virtual IADataDriversLibPtr create_appropriate_lib(
      IADataDriversContextPtr ctx);

  /**
   * @brief Creates the default annotations database implementation carrying
   * the library compile time application name and version defaults.
   *
   * @return Returns the default annotations database implementation. Currently
   * returns the iannotator::dbs::annotations::AnnotationsDirDB class instance.
   */
  virtual IAnnotationsDBPtr create_annotations_db();

  /**
   * @brief Creates the annotations database implementation configured with
   * the data provided by the given context: the application identity, the
   * records factory and the merger are taken from it when set.
   *
   * @param ctx The context to configure the new database with. A nullptr is
   * treated the same way as the argument less overload call.
   *
   * @return Returns the configured annotations database implementation.
   */
  virtual IAnnotationsDBPtr create_annotations_db(IADataDriversContextPtr ctx);

  /**
   * @brief Creates the default data records factory implementation.
   *
   * @return Returns the default records factory implementation. Currently
   * returns the iannotator::records::RecordsFactory class instance.
   */
  virtual IRecordsFactoryPtr create_records_factory();

  /**
   * @brief Creates the default annotations databases merger implementation.
   *
   * @return Returns the default merger implementation. Currently returns the
   * iannotator::dbs::annotations::merger::DefaultDBMerger class instance.
   */
  virtual IDBMergerPtr create_db_merger();

  static LibFactoryPtr create_factory();
};

using LibFactoryPtr = LibFactory::LibFactoryPtr;

}  // namespace lib0impl

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H
