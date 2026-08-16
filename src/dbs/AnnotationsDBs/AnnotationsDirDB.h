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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H

#include <memory>
#include <string>

#include "IAnnotationsDB.h"
#include "IImagesPathsDBProvider.h"
#include "ImageRecord.h"
#include "project-global-decls.h"
#include "src/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/dbs/AnnotationsDBs/LibraryImpl.h"
#include "src/helpers/SortHelper.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db controller.
 *
 * The class holds the records in memory and knows nothing of the format they
 * were read from: the project file reading and writing belongs to the
 * LibraryImpl driver below, which the load_db() and the store_db() calls are
 * performed through.
 */
class AnnotationsDirDB
    : public virtual AnnotationsDBTypes,
      virtual public ImagesAnnotatorDataDrivers011::IAnnotationsDB,
      virtual public helpers::SortHelper
{
 public:
  using AnnotationsList =
      ImagesAnnotatorDataDrivers011::IImagesPathsDBProvider::AnnotationsList;

  /// @brief Disambiguates the equal aliases inherited from the several bases
  using ImageRecordPtr = AnnotationsDBTypes::ImageRecordPtr;
  using IImagesPathsDBProviderPtr =
      AnnotationsDBTypes::IImagesPathsDBProviderPtr;
  using IAnnotationsDBPtr = AnnotationsDBTypes::IAnnotationsDBPtr;

  using AnnotationsDirDBPtr = std::shared_ptr<AnnotationsDirDB>;

  virtual ~AnnotationsDirDB() = default;
  AnnotationsDirDB();

  static AnnotationsDirDBPtr create();

  virtual bool load_db(const std::string& fpath) override;

  virtual ImageRecordsSet& get_images_db() override;
  virtual std::string get_db_path() override;
  virtual AnnotationsList get_available_annotations() override;

  virtual bool changed() override;

  virtual bool delete_image_record(const std::string& irFullPath) override;

  virtual void add_images_db(const ImageRecordsSet& andb) override;

  virtual bool store_db() override;
  virtual bool store_db(const std::string& fpath) override;

  virtual void set_app_info(const std::string& name,
                            const std::string& version) override;
  virtual std::string get_app_name() const override;
  virtual std::string get_app_version() const override;

  /**
   * @brief Sets the records factory the loaded records are built with.
   *
   * @param factory The factory to use. A nullptr restores the converters own
   * default one.
   */
  virtual void set_records_factory(IRecordsFactoryPtr factory);

  /**
   * @brief Sets the merger the add_images_db() calls are performed with.
   *
   * @param merger The merger to use. A nullptr restores the library default
   * one.
   */
  virtual void set_db_merger(IDBMergerPtr merger);

  /**
   * @brief Sets the project file driver the load_db() and the store_db()
   * calls are performed through, which is what a project file format other
   * than the JSON one is introduced with.
   *
   * @param newImpl The driver to use. A nullptr is ignored, since the database
   * has to keep one.
   */
  virtual void set_library_impl(LibraryImplPtr newImpl);

 protected:
  void update_current_last_saved();
  void update_current_last_saved_no_check();

  ImageRecordsSet irdb;
  std::string current_db_path;

  ImageRecordsSet last_saved_db;

  /// @brief The project file driver every load_db() and store_db() goes
  /// through. Defaults to the JSON format one.
  LibraryImplPtr impl;

  /// @brief The injected databases merger. A nullptr makes the add_images_db()
  /// use the merger::DefaultDBMerger one.
  IDBMergerPtr db_merger;
};

using AnnotationsDirDBPtr = AnnotationsDirDB::AnnotationsDirDBPtr;

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
