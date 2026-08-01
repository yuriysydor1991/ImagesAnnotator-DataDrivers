#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYCONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYCONTEXT_CLASS_H

#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

namespace ImagesAnnotatorDataDrivers011
{

/**
 * @brief The unit tests stand-in for the real LibraryContext.
 *
 * Field for field identical to the installable one, except that the
 * constructor is inline here. The real context seeds its defaults from the
 * CMake generated project-global-decls.h through an out-of-line constructor
 * living in LibraryContext.cpp, and a unit test that only compiles a single
 * translation unit must not have to drag that object file in.
 */
class LibraryContext
{
 public:
  using LibraryContextPtr = std::shared_ptr<LibraryContext>;

  virtual ~LibraryContext() = default;
  LibraryContext() = default;

  std::string db_path;

  std::string app_name{"ImagesAnnotator"};
  std::string app_version{"0.11.0"};

  IRecordsFactoryPtr records_factory;
  IDBMergerPtr merger;

  IAnnotationsDBPtr db;

  inline static LibraryContextPtr create()
  {
    return std::make_shared<LibraryContext>();
  }
};

using LibraryContextPtr = LibraryContext::LibraryContextPtr;

}  // namespace ImagesAnnotatorDataDrivers011

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBRARYCONTEXT_CLASS_H
