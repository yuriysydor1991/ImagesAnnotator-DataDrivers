#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H

#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

namespace ImagesAnnotatorDataDrivers012
{

/**
 * @brief The unit tests stand-in for the real IADataDriversContext.
 *
 * Field for field identical to the installable one, except that the
 * constructor is inline here. The real context seeds its defaults from the
 * CMake generated project-global-decls.h through an out-of-line constructor
 * living in IADataDriversContext.cpp, and a unit test that only compiles a
 * single translation unit must not have to drag that object file in.
 */
class IADataDriversContext
{
 public:
  using IADataDriversContextPtr = std::shared_ptr<IADataDriversContext>;

  virtual ~IADataDriversContext() = default;
  IADataDriversContext() = default;

  std::string db_path;

  std::string app_name{"ImagesAnnotator"};
  std::string app_version{"0.12.0"};

  IRecordsFactoryPtr records_factory;
  IDBMergerPtr merger;

  IAnnotationsDBPtr db;

  inline static IADataDriversContextPtr create()
  {
    return std::make_shared<IADataDriversContext>();
  }
};

using IADataDriversContextPtr = IADataDriversContext::IADataDriversContextPtr;

}  // namespace ImagesAnnotatorDataDrivers012

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_IADATADRIVERSCONTEXT_CLASS_H
