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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_URL_HELPER_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_URL_HELPER_CLASS_H

#include <string>

#include "src/helpers/IHelper.h"

namespace helpers
{

/**
 * @brief The helper class to recognise and to resolve the image records URLs.
 *
 * The library stores an image either as a local directory plus a relative
 * path, or as a web page URL plus a possibly relative link found on it. Both
 * have to collapse into one comparable string, otherwise the records merging
 * would treat the same remote image reached through two spellings as two
 * distinct entries.
 *
 * The resolution is delegated to the libcurl URL API (CURLU), which is the
 * only reason this library links libcurl at all - nothing here downloads
 * anything.
 */
class UrlHelper : virtual public IHelper
{
 public:
  virtual ~UrlHelper() = default;
  UrlHelper() = default;

  /**
   * @brief Tells whether the given string looks like a URL rather than a
   * filesystem path.
   *
   * @return Returns true when the string contains the "://" scheme separator.
   */
  static bool is_url(const std::string& maybe);

  /**
   * @brief Resolves the relative path against the given host path.
   *
   * @param hostPath The base URL to resolve against.
   * @param relPath The relative (or absolute) link to resolve.
   *
   * @return Returns the resolved absolute URL, or an empty string on any
   * libcurl resolution failure.
   */
  static std::string get_absolute_url(const std::string& hostPath,
                                      const std::string& relPath);
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_URL_HELPER_CLASS_H
