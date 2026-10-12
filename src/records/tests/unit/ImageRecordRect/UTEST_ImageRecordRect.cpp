#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "ImageRecordRect.h"

using namespace testing;
using ImagesAnnotatorDataDrivers013::ImageRecordRect;
using ImagesAnnotatorDataDrivers013::ImageRecordRectPtr;
using ImagesAnnotatorDataDrivers013::ImageRecordRectSet;

namespace
{

ImageRecordRectPtr make_rect(const std::string& name, const int x, const int y,
                             const int width, const int height)
{
  return std::make_shared<ImageRecordRect>(name, x, y, width, height);
}

}  // namespace

TEST(UTEST_ImageRecordRect, default_constructed_rect_is_unnamed_and_zeroed)
{
  ImageRecordRect rct;

  EXPECT_TRUE(rct.name.empty());
  EXPECT_EQ(rct.x, 0);
  EXPECT_EQ(rct.y, 0);
  EXPECT_EQ(rct.width, 0);
  EXPECT_EQ(rct.height, 0);
}

TEST(UTEST_ImageRecordRect, geometry_constructor_keeps_every_argument)
{
  ImageRecordRect rct{"cat", 1, 2, 30, 40};

  EXPECT_EQ(rct.name, "cat");
  EXPECT_EQ(rct.x, 1);
  EXPECT_EQ(rct.y, 2);
  EXPECT_EQ(rct.width, 30);
  EXPECT_EQ(rct.height, 40);
}

TEST(UTEST_ImageRecordRect, geometry_constructor_accepts_negative_coordinates)
{
  ImageRecordRect rct{"dog", -5, -6, 7, 8};

  EXPECT_EQ(rct.x, -5);
  EXPECT_EQ(rct.y, -6);
  EXPECT_EQ(rct.width, 7);
  EXPECT_EQ(rct.height, 8);
}

TEST(UTEST_ImageRecordRect, duplicate_shared_returns_a_distinct_equal_object)
{
  auto orig = make_rect("dog", 5, 6, 7, 8);

  auto dup = orig->duplicate_shared();

  ASSERT_NE(dup, nullptr);
  EXPECT_NE(dup.get(), orig.get());
  EXPECT_TRUE(ImageRecordRect::equal(orig, dup));
  EXPECT_EQ(dup->name, orig->name);
}

TEST(UTEST_ImageRecordRect, duplicate_shared_copy_is_detached_from_the_source)
{
  auto orig = make_rect("dog", 5, 6, 7, 8);

  auto dup = orig->duplicate_shared();

  dup->name = "cat";
  dup->x = 100;
  dup->height = 200;

  EXPECT_EQ(orig->name, "dog");
  EXPECT_EQ(orig->x, 5);
  EXPECT_EQ(orig->height, 8);
  EXPECT_FALSE(ImageRecordRect::equal(orig, dup));
}

TEST(UTEST_ImageRecordRect, equal_ptr_short_circuits_on_the_very_same_pointer)
{
  auto rct = make_rect("x", 1, 1, 1, 1);

  EXPECT_TRUE(ImageRecordRect::equal(rct, rct));
}

TEST(UTEST_ImageRecordRect, equal_ptr_of_two_empty_pointers_is_true)
{
  // Both hold the very same (null) raw pointer, so the short circuit answers
  // without dereferencing anything.
  const ImageRecordRectPtr l;
  const ImageRecordRectPtr r;

  EXPECT_TRUE(ImageRecordRect::equal(l, r));
}

TEST(UTEST_ImageRecordRect, equal_ptr_compares_distinct_objects_value_wise)
{
  EXPECT_TRUE(ImageRecordRect::equal(make_rect("x", 1, 2, 3, 4),
                                     make_rect("x", 1, 2, 3, 4)));
}

TEST(UTEST_ImageRecordRect, equal_ptr_detects_each_differing_field)
{
  const auto base = make_rect("x", 1, 2, 3, 4);

  EXPECT_FALSE(ImageRecordRect::equal(base, make_rect("y", 1, 2, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, make_rect("x", 9, 2, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, make_rect("x", 1, 9, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, make_rect("x", 1, 2, 9, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, make_rect("x", 1, 2, 3, 9)));
}

TEST(UTEST_ImageRecordRect, equal_set_short_circuits_on_the_same_set_reference)
{
  ImageRecordRectSet single{make_rect("a", 0, 0, 1, 1)};

  EXPECT_TRUE(ImageRecordRect::equal(single, single));
}

TEST(UTEST_ImageRecordRect, equal_set_of_two_empty_sets_is_true)
{
  const ImageRecordRectSet l;
  const ImageRecordRectSet r;

  EXPECT_TRUE(ImageRecordRect::equal(l, r));
}

TEST(UTEST_ImageRecordRect, equal_set_compares_the_elements_value_wise)
{
  const ImageRecordRectSet l{make_rect("a", 0, 0, 1, 1),
                             make_rect("b", 1, 1, 2, 2)};
  const ImageRecordRectSet r{make_rect("a", 0, 0, 1, 1),
                             make_rect("b", 1, 1, 2, 2)};

  EXPECT_TRUE(ImageRecordRect::equal(l, r));

  const ImageRecordRectSet differing{make_rect("a", 0, 0, 1, 1),
                                     make_rect("B", 1, 1, 2, 2)};

  EXPECT_FALSE(ImageRecordRect::equal(l, differing));
}

TEST(UTEST_ImageRecordRect, equal_set_is_order_sensitive)
{
  const auto first = make_rect("a", 0, 0, 1, 1);
  const auto second = make_rect("b", 1, 1, 2, 2);

  const ImageRecordRectSet straight{first, second};
  const ImageRecordRectSet reversed{second, first};

  EXPECT_FALSE(ImageRecordRect::equal(straight, reversed));
}

TEST(UTEST_ImageRecordRect, equal_set_of_different_sizes_is_false)
{
  const ImageRecordRectSet longer{make_rect("a", 0, 0, 1, 1),
                                  make_rect("b", 1, 1, 2, 2)};
  const ImageRecordRectSet shorter{make_rect("a", 0, 0, 1, 1)};

  EXPECT_FALSE(ImageRecordRect::equal(longer, shorter));
  EXPECT_FALSE(ImageRecordRect::equal(shorter, longer));
  EXPECT_FALSE(ImageRecordRect::equal(shorter, ImageRecordRectSet{}));
}
