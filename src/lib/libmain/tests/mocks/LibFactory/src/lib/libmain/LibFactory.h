#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <set>

#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "ILib.h"
#include "IRecordsFactory.h"
#include "LibraryContext.h"

namespace lib0impl
{

/**
 * @brief Pure abstract stand-in for the real LibFactory, so that every
 * factory method can be mocked with an (override) MOCK_METHOD.
 */
class LibFactorySynthParent
{
 public:
  using ILibPtr = ImagesAnnotatorDataDrivers011::ILibPtr;
  using LibraryContextPtr = ImagesAnnotatorDataDrivers011::LibraryContextPtr;
  using LibraryContext = ImagesAnnotatorDataDrivers011::LibraryContext;
  using IAnnotationsDBPtr = ImagesAnnotatorDataDrivers011::IAnnotationsDBPtr;
  using IRecordsFactoryPtr = ImagesAnnotatorDataDrivers011::IRecordsFactoryPtr;
  using IDBMergerPtr = ImagesAnnotatorDataDrivers011::IDBMergerPtr;

  virtual ~LibFactorySynthParent() = default;
  LibFactorySynthParent() = default;

  virtual ILibPtr create_default_lib() = 0;
  virtual LibraryContextPtr create_default_context() = 0;
  virtual ILibPtr create_appropriate_lib(LibraryContextPtr ctx) = 0;
  virtual IAnnotationsDBPtr create_annotations_db() = 0;
  virtual IAnnotationsDBPtr create_annotations_db(LibraryContextPtr ctx) = 0;
  virtual IRecordsFactoryPtr create_records_factory() = 0;
  virtual IDBMergerPtr create_db_merger() = 0;
};

class LibFactory : public LibFactorySynthParent
{
 public:
  using LibFactoryPtr = std::shared_ptr<LibFactory>;

  virtual ~LibFactory() = default;
  LibFactory()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(LibFactory& instance)> onMockCreate;

  MOCK_METHOD(ILibPtr, create_default_lib, (), (override));
  MOCK_METHOD(LibraryContextPtr, create_default_context, (), (override));
  MOCK_METHOD(ILibPtr, create_appropriate_lib, (LibraryContextPtr ctx),
              (override));
  MOCK_METHOD(IAnnotationsDBPtr, create_annotations_db, (), (override));
  MOCK_METHOD(IAnnotationsDBPtr, create_annotations_db, (LibraryContextPtr ctx),
              (override));
  MOCK_METHOD(IRecordsFactoryPtr, create_records_factory, (), (override));
  MOCK_METHOD(IDBMergerPtr, create_db_merger, (), (override));

  inline static LibFactoryPtr create_factory()
  {
    return std::make_shared<LibFactory>();
  }
};

using LibFactoryPtr = LibFactory::LibFactoryPtr;

}  // namespace lib0impl

#endif  // IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H
