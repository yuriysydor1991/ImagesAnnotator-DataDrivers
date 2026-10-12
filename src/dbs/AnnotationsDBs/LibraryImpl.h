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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_LIBRARY_IMPL_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_LIBRARY_IMPL_CLASS_H

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "src/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotations project file driver, implementing the JSON project
 * file format.
 *
 * Everything the library knows about the format of a project file lives here:
 * the reading of such a file into an ImageRecordsSet, the writing of that set
 * back out and the application identity stamped into - and expected inside -
 * the produced files. AnnotationsDirDB holds the records in memory and no
 * longer knows the format they arrived in, so a project file format other than
 * the JSON one is a descendant of this class rather than a change of the
 * database: the load() and the store() of every format parse, validate,
 * convert and print the very JSON project text, the read_text() and the
 * write_text() of a descendant keep that text in a file of its own format (see
 * the Imgajz1LibraryImpl of the gzip compressed imgajz1 files).
 */
class LibraryImpl : public virtual AnnotationsDBTypes
{
 public:
  using LibraryImplPtr = std::shared_ptr<LibraryImpl>;

  virtual ~LibraryImpl() = default;
  LibraryImpl() = default;

  static LibraryImplPtr create();

  /**
   * @brief Reads the given project file and replaces the given records set
   * with the one it holds.
   *
   * @param fpath The project file path to read.
   * @param irdb The records set to fill in. It is left alone unless the whole
   * of the file was read.
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool load(const std::string& fpath, ImageRecordsSet& irdb);

  /**
   * @brief Writes the given records set into the given project file path.
   *
   * @param fpath The project file path to write.
   * @param irdb The records set to store.
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool store(const std::string& fpath, const ImageRecordsSet& irdb);

  /**
   * @brief Overrides the application identity the produced files carry.
   *
   * @param name The JSON root element name.
   * @param version The version string to write under the root element.
   */
  virtual void set_app_info(const std::string& name,
                            const std::string& version);

  /// @brief Reports the JSON root element name currently in use
  virtual std::string get_app_name() const;
  /// @brief Reports the version string currently written into the files
  virtual std::string get_app_version() const;

  /**
   * @brief Sets the records factory the loaded records are built with.
   *
   * @param factory The factory to use. A nullptr restores the converters own
   * default one.
   */
  virtual void set_records_factory(IRecordsFactoryPtr factory);

  /**
   * @brief Reports the records factory the loaded records are built with.
   *
   * @return Returns the factory set or a nullptr for the converters own
   * default one.
   */
  virtual IRecordsFactoryPtr get_records_factory() const;

 protected:
  /**
   * @brief Reads the JSON text of the project out of the given file: the
   * whole text of the file.
   *
   * @param fpath The project file path to read.
   * @param text Out: the JSON text of the project.
   *
   * @return Returns true on the success and false for the file unable to be
   * read.
   */
  virtual bool read_text(const std::string& fpath, std::string& text);

  /**
   * @brief Writes the JSON text of the project into the given file: the whole
   * text of the file.
   *
   * @param fpath The project file path to write.
   * @param text The JSON text of the project, pretty printed with the two space
   * indentation.
   *
   * @return Returns true on the success and false for the file unable to be
   * written.
   */
  virtual bool write_text(const std::string& fpath, const std::string& text);

  virtual bool serialize(const nlohmann::json& json);
  virtual ImageRecordsSet load_the_irs(const nlohmann::json& json);

  /// @brief The JSON root element name of the produced project files
  std::string app_name{AnnotationsJSONSerializator::default_root_app_name};
  /// @brief The version string written into the produced project files
  std::string app_version{AnnotationsJSONSerializator::default_app_version};

  /// @brief The injected records factory. A nullptr makes the converters use
  /// their own default one.
  IRecordsFactoryPtr records_factory;
};

using LibraryImplPtr = LibraryImpl::LibraryImplPtr;

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_LIBRARY_IMPL_CLASS_H
