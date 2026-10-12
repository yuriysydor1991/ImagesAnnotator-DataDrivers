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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORD_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORD_CLASS_H

#include <filesystem>
#include <string>
#include <vector>

#include "IRecord.h"
#include "ImageRecordRect.h"

namespace ImagesAnnotatorDataDrivers013
{

/**
 * @brief The particular annotated image record class.
 *
 * Holds everything the project JSON database keeps about a single image:
 * where it came from, its intrinsic size, the preview scale and the set of
 * the drawn annotation rectangles.
 *
 * Current file is a target for the library header installation.
 */
class ImageRecord : virtual public IRecord
{
 public:
  using ImageRecordPtr = std::shared_ptr<ImageRecord>;
  using ImageRecordsSet = std::vector<ImageRecordPtr>;

  inline static constexpr const double defaultScaleStep = 0.05;

  /// @brief Bounds for the user-controlled zoom. scaleStepOut() never drives
  /// the scale to zero or below (which would produce a degenerate scaled pixbuf
  /// in the presentation layer), and scaleStepIn() never magnifies past a sane
  /// maximum. minScale stays strictly positive so it never collides with the
  /// "0 == no user-selected scale yet" sentinel.
  inline static constexpr const double minScale = 0.05;
  inline static constexpr const double maxScale = 20.0;

  virtual ~ImageRecord() = default;
  ImageRecord(const std::string& npath = "", const std::string& nabspath = "");

  /// @brief The root directory (or the web page URL) this record was taken from
  std::string abs_dir_path;

  /// @brief rel path to the image starting from abs_dir_path
  std::string path;

  /**
   * @brief If the image is loaded from the network - this is it's temporary
   * cache filepath. Not included into the projects JSON files.
   */
  std::filesystem::path tmppath;

  ImageRecordRectSet rects;
  ImageRecordRectPtr current_rect;

  /// @brief Image width and height respectively
  int iwidth{0};
  int iheight{0};

  /// @brief The per-image preview scale. A value of 0 means "no user-selected
  /// scale yet": the presentation layer computes the initial fit-to-window
  /// scale on first display, and any value set afterwards (that fit or a
  /// subsequent user zoom) is preserved across image switches and serialized
  /// to the project JSON.
  double imageScale{0.0};

  void scaleStepIn();
  void scaleStepOut();

  /**
   * @brief Computes the path the image is to be read from.
   *
   * @return Returns the temporary cache path when the image was preloaded from
   * the network, otherwise abs_dir_path joined with path.
   */
  std::string get_full_path() const;

  /**
   * @brief Erases the currently selected rectangle from the rects set.
   *
   * @return Returns true when the current rectangle was found and erased.
   */
  bool erase_current_rect();

  /**
   * @brief Compares two image record sets value wise.
   *
   * @return Returns true when both sets hold equal records in equal order.
   */
  static bool equal(const ImageRecordsSet& l, const ImageRecordsSet& r);

  /**
   * @brief Compares two image records value wise, rectangles included.
   *
   * @return Returns true when both records hold the same data.
   */
  static bool equal(const ImageRecordPtr& l, const ImageRecordPtr& r);

  /**
   * @brief Creates a detached deep copy of the current record.
   *
   * @return Returns a new instance holding copies of the own rectangles.
   */
  ImageRecordPtr duplicate_shared() const;

  /**
   * @brief Creates a detached deep copy of the whole given records set.
   *
   * @return Returns a new set of the freshly duplicated records.
   */
  static ImageRecordsSet duplicate(const ImageRecordsSet& orig);

  static ImageRecordPtr create();
  static ImageRecordPtr create(const std::string& npath,
                               const std::string& nabspath);
};

using ImageRecordPtr = ImageRecord::ImageRecordPtr;
using ImageRecordsSet = ImageRecord::ImageRecordsSet;

}  // namespace ImagesAnnotatorDataDrivers013

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IMAGERECORD_CLASS_H
