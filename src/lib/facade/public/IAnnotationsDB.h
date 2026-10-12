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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IANNOTATIONSDB_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IANNOTATIONSDB_ABSTRACT_CLASS_H

#include <memory>
#include <string>

#include "IImagesPathsDBProvider.h"
#include "ImageRecord.h"

namespace ImagesAnnotatorDataDrivers012
{

/**
 * @brief The annotations project database interface.
 *
 * This is the central abstraction of the library: an annotations database
 * backed by a single project file. It extends the read only
 * IImagesPathsDBProvider with the loading, merging and storing operations.
 *
 * The file name extension of the project file path tells its format: the path
 * of the .imgajz1 extension (of any letter case) is the gzip compressed imgajz1
 * file of the JSON project document, any other path is the plain JSON file of
 * it. So storing a database loaded out of a .json file into a .imgajz1 one
 * converts the project, and the other way round.
 *
 * Obtain an instance through IADataDriversFacade::create_annotations_db() or
 * IADataDriversFacade::open_annotations_db().
 *
 * Current file is a target for the library header installation.
 */
class IAnnotationsDB : virtual public IImagesPathsDBProvider
{
 public:
  using IAnnotationsDBPtr = std::shared_ptr<IAnnotationsDB>;

  virtual ~IAnnotationsDB() = default;
  IAnnotationsDB() = default;

  /**
   * @brief Parses the given project file and replaces the in-memory records
   * with the ones read from it.
   *
   * On success the given path also becomes the default destination of the
   * argument less store_db() overload.
   *
   * @param fpath The project file path to read: the imgajz1 one of the
   * .imgajz1 file name extension, the JSON one of any other.
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool load_db(const std::string& fpath) = 0;

  /**
   * @brief Writes the in-memory records back to the last used project file
   * path.
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool store_db() = 0;

  /**
   * @brief Writes the in-memory records into the given project file path,
   * which also becomes the new default destination.
   *
   * @param fpath The project file path to write: the imgajz1 one of the
   * .imgajz1 file name extension, the JSON one of any other.
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool store_db(const std::string& fpath) = 0;

  /**
   * @brief Merges the given records into the held ones, skipping the
   * duplicates and keeping the result sorted by the records full paths.
   *
   * @param andb The records set to append.
   */
  virtual void add_images_db(const ImageRecordsSet& andb) = 0;

  /**
   * @brief Overrides the application identity written into (and expected
   * inside) the project JSON files.
   *
   * The name becomes the JSON root element of the produced files and the
   * version is stored under it. Both default to the compile time values
   * described in IADataDriversContext, which keep the format compatible with
   * the ImagesAnnotator application files.
   *
   * @param name The JSON root element name.
   * @param version The version string to write under the root element.
   */
  virtual void set_app_info(const std::string& name,
                            const std::string& version) = 0;

  /**
   * @brief Reports the JSON root element name currently in use.
   *
   * @return Returns the configured application name.
   */
  virtual std::string get_app_name() const = 0;

  /**
   * @brief Reports the version string currently written into the files.
   *
   * @return Returns the configured application version.
   */
  virtual std::string get_app_version() const = 0;
};

using IAnnotationsDBPtr = IAnnotationsDB::IAnnotationsDBPtr;

}  // namespace ImagesAnnotatorDataDrivers012

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IANNOTATIONSDB_ABSTRACT_CLASS_H
