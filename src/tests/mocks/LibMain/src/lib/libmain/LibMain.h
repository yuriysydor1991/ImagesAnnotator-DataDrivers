#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H

#include <gmock/gmock.h>

#include <memory>

#include "IADataDriversContext.h"
#include "IADataDriversLib.h"

namespace lib0impl
{

class LibMain : public ImagesAnnotatorDataDrivers012::IADataDriversLib
{
 public:
  using LibMainPtr = std::shared_ptr<LibMain>;
  using IADataDriversContextPtr =
      ImagesAnnotatorDataDrivers012::IADataDriversContextPtr;

  virtual ~LibMain() = default;
  LibMain() = default;

  MOCK_METHOD(bool, libcall, (IADataDriversContextPtr ctx), (override));

  inline static LibMainPtr create() { return std::make_shared<LibMain>(); }
};

using LibMainPtr = LibMain::LibMainPtr;

}  // namespace lib0impl

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYMAIN_CLASS_H
