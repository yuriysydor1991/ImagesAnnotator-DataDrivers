#ifndef IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_DATA_DRIVERS_PROJECT_LIBFACTORY_CLASS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <set>

#include "IADataDriversContext.h"
#include "IADataDriversLib.h"
#include "IAnnotationsDB.h"
#include "IDBMerger.h"
#include "IRecordsFactory.h"

namespace lib0impl
{

/**
 * @brief Pure abstract stand-in for the real LibFactory, so that every
 * factory method can be mocked with an (override) MOCK_METHOD.
 */
class LibFactorySynthParent
{
 public:
  using IADataDriversLibPtr =
      ImagesAnnotatorDataDrivers014::IADataDriversLibPtr;
  using IADataDriversContextPtr =
      ImagesAnnotatorDataDrivers014::IADataDriversContextPtr;
  using IADataDriversContext =
      ImagesAnnotatorDataDrivers014::IADataDriversContext;
  using IAnnotationsDBPtr = ImagesAnnotatorDataDrivers014::IAnnotationsDBPtr;
  using IRecordsFactoryPtr = ImagesAnnotatorDataDrivers014::IRecordsFactoryPtr;
  using IDBMergerPtr = ImagesAnnotatorDataDrivers014::IDBMergerPtr;

  virtual ~LibFactorySynthParent() = default;
  LibFactorySynthParent() = default;

  virtual IADataDriversLibPtr create_default_lib() = 0;
  virtual IADataDriversContextPtr create_default_context() = 0;
  virtual IADataDriversLibPtr create_appropriate_lib(
      IADataDriversContextPtr ctx) = 0;
  virtual IAnnotationsDBPtr create_annotations_db() = 0;
  virtual IAnnotationsDBPtr create_annotations_db(
      IADataDriversContextPtr ctx) = 0;
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

  MOCK_METHOD(IADataDriversLibPtr, create_default_lib, (), (override));
  MOCK_METHOD(IADataDriversContextPtr, create_default_context, (), (override));
  MOCK_METHOD(IADataDriversLibPtr, create_appropriate_lib,
              (IADataDriversContextPtr ctx), (override));
  MOCK_METHOD(IAnnotationsDBPtr, create_annotations_db, (), (override));
  MOCK_METHOD(IAnnotationsDBPtr, create_annotations_db,
              (IADataDriversContextPtr ctx), (override));
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
