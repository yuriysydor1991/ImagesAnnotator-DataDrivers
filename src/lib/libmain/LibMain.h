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

#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H

#include <memory>

#include "IADataDriversContext.h"
#include "IADataDriversLib.h"

namespace lib0impl
{

/**
 * @brief The default library implementation class.
 * Class is designed to hold the main library implementation code, which
 * is the one shot annotations project database opening: the libcall method
 * builds the database in accordance of the given context data and hands it
 * back through that very same context.
 */
class LibMain : public ImagesAnnotatorDataDrivers014::IADataDriversLib
{
 public:
  using LibMainPtr = std::shared_ptr<LibMain>;
  using IADataDriversContextPtr =
      ImagesAnnotatorDataDrivers014::IADataDriversContextPtr;

  virtual ~LibMain() = default;
  LibMain();

  /**
   * @brief The implemented library interface method derived from an
   * IADataDriversLib abstract class. Designed to contain the main library code
   * implementation.
   *
   * Creates the annotations database instance with the given context data
   * and, in case the context db_path is not empty, loads the pointed project
   * JSON file into it. The resulting instance is provided back through the
   * context db field.
   *
   * @param ctx A filled IADataDriversContext instance with appropriate data
   * to perform it's actions.
   *
   * @return Returns a true value on the success and false in case of any error.
   */
  virtual bool libcall(IADataDriversContextPtr ctx) override;

  static LibMainPtr create();
};

using LibMainPtr = LibMain::LibMainPtr;

}  // namespace lib0impl

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H
