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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ1_LIBRARY_IMPL_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ1_LIBRARY_IMPL_CLASS_H

#include <memory>
#include <string>

#include "src/dbs/AnnotationsDBs/LibraryImpl.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotations project file driver of the imgajz1 format.
 *
 * The imgajz1 file (the images annotator JSON zipped file of the first
 * version) is the regular JSON project file, gzip compressed by the
 * ZlibController: the very JSON text the JSON project file of the same project
 * keeps. So the stock gzip tools convert the two files into each other:
 *
 * @code
 * gzip -dc project.imgajz1 > project.json
 * gzip -c project.json > project.imgajz1
 * @endcode
 *
 * Everything but the compression is the JSON format of the LibraryImpl, whose
 * reading and writing of the text of the file alone the driver overrides. The
 * file of no gzip compression is read as it is, the way the ZlibController
 * reads it.
 */
class Imgajz1LibraryImpl : public LibraryImpl
{
 public:
  using Imgajz1LibraryImplPtr = std::shared_ptr<Imgajz1LibraryImpl>;

  virtual ~Imgajz1LibraryImpl() = default;
  Imgajz1LibraryImpl() = default;

  static Imgajz1LibraryImplPtr create();

  /**
   * @brief Creates the driver of the application identity and of the records
   * factory of the given one, so the project of the files of another format
   * goes on in the imgajz1 files.
   *
   * @param like The driver to take the settings of.
   *
   * @return Returns the new driver.
   */
  static Imgajz1LibraryImplPtr create(const LibraryImpl& like);

  /**
   * @brief Tells the imgajz1 file by the file name extension of its path, the
   * .imgajz1 of any letter case.
   *
   * @param fpath The project file path.
   *
   * @return Returns true for the path of the imgajz1 file.
   */
  static bool is_imgajz1_path(const std::string& fpath);

  /// @brief The file name extension the imgajz1 files are told by
  inline static const std::string file_extension{".imgajz1"};

 protected:
  /// @brief Decompresses the JSON text of the project out of the file.
  virtual bool read_text(const std::string& fpath, std::string& text) override;

  /// @brief Compresses the JSON text of the project into the file, at the
  /// default zlib level.
  virtual bool write_text(const std::string& fpath,
                          const std::string& text) override;
};

using Imgajz1LibraryImplPtr = Imgajz1LibraryImpl::Imgajz1LibraryImplPtr;

}  // namespace iannotator::dbs::annotations

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMGAJZ1_LIBRARY_IMPL_CLASS_H
