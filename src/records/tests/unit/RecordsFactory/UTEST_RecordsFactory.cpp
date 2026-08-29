#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/records/RecordsFactory.h"

using namespace testing;
using iannotator::records::RecordsFactory;
using ImagesAnnotatorDataDrivers012::ImageRecordPtr;
using ImagesAnnotatorDataDrivers012::ImageRecordRect;
using ImagesAnnotatorDataDrivers012::IRecordsFactory;
using ImagesAnnotatorDataDrivers012::IRecordsFactoryPtr;

class UTEST_RecordsFactory : public Test
{
 public:
  UTEST_RecordsFactory() : factory{RecordsFactory::create()} {}

  iannotator::records::RecordsFactoryPtr factory;
};

TEST_F(UTEST_RecordsFactory, create_gives_a_fresh_usable_factory)
{
  ASSERT_NE(factory, nullptr);

  const auto another = RecordsFactory::create();

  ASSERT_NE(another, nullptr);
  EXPECT_NE(another.get(), factory.get());
}

TEST_F(UTEST_RecordsFactory, create_image_record_forwards_both_arguments)
{
  const auto ir = factory->create_image_record("rel.png", "/abs");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "rel.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs");
  EXPECT_EQ(ir->get_full_path(), "/abs/rel.png");
}

TEST_F(UTEST_RecordsFactory, create_image_record_without_arguments_is_empty)
{
  const auto ir = factory->create_image_record();

  ASSERT_NE(ir, nullptr);
  EXPECT_TRUE(ir->path.empty());
  EXPECT_TRUE(ir->abs_dir_path.empty());
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(ir->current_rect, nullptr);
}

TEST_F(UTEST_RecordsFactory, create_image_record_gives_a_new_record_every_time)
{
  const auto first = factory->create_image_record("a.png", "/abs");
  const auto second = factory->create_image_record("a.png", "/abs");

  EXPECT_NE(first.get(), second.get());
}

TEST_F(UTEST_RecordsFactory, create_rect_without_a_record_uses_the_default_name)
{
  const auto rct = factory->create_image_rect_record(nullptr);

  ASSERT_NE(rct, nullptr);
  EXPECT_EQ(rct->name, IRecordsFactory::default_name_record);
  EXPECT_EQ(rct->x, 0);
  EXPECT_EQ(rct->y, 0);
  EXPECT_EQ(rct->width, 0);
  EXPECT_EQ(rct->height, 0);
}

TEST_F(UTEST_RecordsFactory, create_rect_defaults_to_no_owning_record)
{
  const auto rct = factory->create_image_rect_record();

  ASSERT_NE(rct, nullptr);
  EXPECT_EQ(rct->name, IRecordsFactory::default_name_record);
}

TEST_F(UTEST_RecordsFactory, create_rect_numbers_the_name_after_the_rects_count)
{
  const auto ir = factory->create_image_record("a.png", "/abs");

  const auto first = factory->create_image_rect_record(ir);

  ASSERT_NE(first, nullptr);
  EXPECT_EQ(first->name,
            IRecordsFactory::default_numbered_name_record_prefix + "0");
  EXPECT_NE(first->name, IRecordsFactory::default_name_record);

  // The factory only builds the rectangle, appending it stays up to the
  // caller - hence the very same name until the record really grows.
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(factory->create_image_rect_record(ir)->name, first->name);

  ir->rects.emplace_back(first);

  EXPECT_EQ(factory->create_image_rect_record(ir)->name,
            IRecordsFactory::default_numbered_name_record_prefix + "1");

  ir->rects.emplace_back(factory->create_image_rect_record(ir));

  EXPECT_EQ(factory->create_image_rect_record(ir)->name,
            IRecordsFactory::default_numbered_name_record_prefix + "2");
}

TEST_F(UTEST_RecordsFactory, create_rect_with_geometry_fills_every_field)
{
  const auto rct = factory->create_image_rect_record("cat", 1, 2, 30, 40);

  ASSERT_NE(rct, nullptr);
  EXPECT_EQ(rct->name, "cat");
  EXPECT_EQ(rct->x, 1);
  EXPECT_EQ(rct->y, 2);
  EXPECT_EQ(rct->width, 30);
  EXPECT_EQ(rct->height, 40);
}

TEST_F(UTEST_RecordsFactory, create_rect_with_geometry_gives_a_new_object)
{
  const auto first = factory->create_image_rect_record("cat", 1, 2, 30, 40);
  const auto second = factory->create_image_rect_record("cat", 1, 2, 30, 40);

  EXPECT_NE(first.get(), second.get());
  EXPECT_TRUE(ImageRecordRect::equal(first, second));
}

TEST_F(UTEST_RecordsFactory, the_factory_serves_through_the_public_interface)
{
  const IRecordsFactoryPtr iface = factory;

  ASSERT_NE(iface, nullptr);

  const ImageRecordPtr ir = iface->create_image_record("rel.png", "/abs");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "rel.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs");

  EXPECT_EQ(iface->create_image_rect_record()->name,
            IRecordsFactory::default_name_record);
  EXPECT_EQ(iface->create_image_rect_record(ir)->name,
            IRecordsFactory::default_numbered_name_record_prefix + "0");

  const auto rct = iface->create_image_rect_record("dog", 5, 6, 7, 8);

  ASSERT_NE(rct, nullptr);
  EXPECT_EQ(rct->name, "dog");
  EXPECT_EQ(rct->width, 7);
}

TEST_F(UTEST_RecordsFactory, the_produced_records_match_directly_built_ones)
{
  const auto ir = factory->create_image_record("a.png", "/abs");
  const auto rct = factory->create_image_rect_record("cat", 1, 2, 3, 4);

  ASSERT_NE(ir, nullptr);
  ASSERT_NE(rct, nullptr);

  EXPECT_TRUE(ImagesAnnotatorDataDrivers012::ImageRecord::equal(
      ir, ImagesAnnotatorDataDrivers012::ImageRecord::create("a.png", "/abs")));
  EXPECT_TRUE(ImageRecordRect::equal(
      rct, std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4)));
}
