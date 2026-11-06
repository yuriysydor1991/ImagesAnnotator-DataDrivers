/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ2_LIBRARY_IMPL_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ2_LIBRARY_IMPL_CLASS_H

#include <memory>
#include <string>

#include "src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotations project file driver of the imgajz2 format.
 *
 * The imgajz2 file (the images annotator JSON zipped file of the second
 * version) is the gzip compressed JSON project file the way the imgajz1 one
 * is, which keeps the segments as well: the polygon of every segment under the
 * "segment" key next to its rect, and the root "format" key declaring the
 * format and the annotation types a file may hold. The JSON and the imgajz1
 * files keep no segments, so the path of an older format is no place for a
 * project of them, and an older reader takes the .imgajz2 file for no project
 * of its own rather than drop the polygons.
 */
class Imgajz2LibraryImpl : public Imgajz1LibraryImpl
{
 public:
  using Imgajz2LibraryImplPtr = std::shared_ptr<Imgajz2LibraryImpl>;

  virtual ~Imgajz2LibraryImpl() = default;
  Imgajz2LibraryImpl() = default;

  static Imgajz2LibraryImplPtr create();

  /**
   * @brief Creates the driver of the application identity and of the records
   * factory of the given one, so the project of the files of another format
   * goes on in the imgajz2 files.
   *
   * @param like The driver to take the settings of.
   *
   * @return Returns the new driver.
   */
  static Imgajz2LibraryImplPtr create(const LibraryImpl& like);

  /**
   * @brief Tells the imgajz2 file by the file name extension of its path, the
   * .imgajz2 of any letter case.
   *
   * @param fpath The project file path.
   *
   * @return Returns true for the path of the imgajz2 file.
   */
  static bool is_imgajz2_path(const std::string& fpath);

  /// @brief The file name extension the imgajz2 files are told by
  inline static const std::string file_extension{".imgajz2"};

 protected:
  virtual std::shared_ptr<converters::ImageRecords2JSONConverter>
  create_writer() const override;

  virtual converters::ImageRecordRectJSON2RecordConverterPtr
  create_rect_reader() const override;
};

using Imgajz2LibraryImplPtr = Imgajz2LibraryImpl::Imgajz2LibraryImplPtr;

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ2_LIBRARY_IMPL_CLASS_H
