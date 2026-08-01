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

#include "src/helpers/UrlHelper.h"

#include <curl/curl.h>

#include <algorithm>
#include <cassert>
#include <string>

#include "src/log/log.h"

namespace
{

/**
 * @brief Performs the resolution inside an already created CURLU context.
 *
 * Kept apart from the public entry point so that the context is cleaned up
 * exactly once, on every return path.
 */
std::string resolve(CURLU* resolved_url, const std::string& hostPath,
                    const std::string& relPath)
{
  if (resolved_url == nullptr) {
    LOGE("Fail to create resolved url context instance");
    return {};
  }

  if (curl_url_set(resolved_url, CURLUPART_URL, hostPath.c_str(), 0) !=
      CURLUE_OK) {
    LOGE("Failure to set the host path");
    return {};
  }

  if (curl_url_set(resolved_url, CURLUPART_URL, relPath.c_str(), 0) !=
      CURLUE_OK) {
    LOGE("Failure to set rel path");
    return {};
  }

  char* absolute_url{nullptr};

  if (curl_url_get(resolved_url, CURLUPART_URL, &absolute_url, 0) !=
          CURLUE_OK &&
      absolute_url != nullptr) {
    LOGE("URL resolution failed");
    return {};
  }

  assert(absolute_url != nullptr);

  std::string finalURL = absolute_url;

  curl_free(absolute_url);

  return finalURL;
}

}  // namespace

namespace helpers
{

bool UrlHelper::is_url(const std::string& maybe)
{
  static const std::string uComp = "://";

  return std::search(maybe.cbegin(), maybe.cend(), uComp.cbegin(),
                     uComp.cend()) != maybe.cend();
}

std::string UrlHelper::get_absolute_url(const std::string& hostPath,
                                        const std::string& relPath)
{
  CURLU* resolved_url = curl_url();

  std::string finalURL = resolve(resolved_url, hostPath, relPath);

  curl_url_cleanup(resolved_url);

  return finalURL;
}

}  // namespace helpers
