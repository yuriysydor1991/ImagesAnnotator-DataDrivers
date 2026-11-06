#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "ImageRecordRect.h"
#include "ImageRecordSegment.h"

using namespace testing;
using ImagesAnnotatorDataDrivers015::ImageRecordRect;
using ImagesAnnotatorDataDrivers015::ImageRecordSegment;
using Points = ImageRecordRect::Points;

TEST(UTEST_ImageRecordSegment, constructor_keeps_the_rectangle_and_the_points)
{
  const ImageRecordSegment s{"cat", 1, 2, 30, 40, {{0.0, 0.5}, {1.0, 1.0}}};

  EXPECT_EQ(s.name, "cat");
  EXPECT_EQ(s.x, 1);
  EXPECT_EQ(s.y, 2);
  EXPECT_EQ(s.width, 30);
  EXPECT_EQ(s.height, 40);
  EXPECT_EQ(s.points, (Points{{0.0, 0.5}, {1.0, 1.0}}));
}

TEST(UTEST_ImageRecordSegment, add_point_grows_the_rectangle_to_the_polygon)
{
  ImageRecordSegment s;

  s.add_point(10.0, 20.0);
  s.add_point(30.0, 20.0);
  s.add_point(20.0, 60.0);

  EXPECT_EQ(s.x, 10);
  EXPECT_EQ(s.y, 20);
  EXPECT_EQ(s.width, 20);
  EXPECT_EQ(s.height, 40);
  EXPECT_EQ(s.points, (Points{{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}}));
}

TEST(UTEST_ImageRecordSegment, add_point_skips_a_point_near_the_last_one)
{
  ImageRecordSegment s;

  s.add_point(10.0, 20.0);
  s.add_point(10.5, 20.5);

  EXPECT_EQ(s.points.size(), 1U);
}

TEST(UTEST_ImageRecordSegment, outline_is_the_polygon_in_the_image_space)
{
  const ImageRecordSegment s{"cat", 10, 20, 20, 40, {{0, 0}, {1, 0}, {0.5, 1}}};

  EXPECT_EQ(s.outline(), (Points{{10.0, 20.0}, {30.0, 20.0}, {20.0, 60.0}}));
}

TEST(UTEST_ImageRecordSegment, resized_rectangle_carries_its_polygon_along)
{
  ImageRecordSegment s{"cat", 10, 20, 20, 40, {{0, 0}, {1, 0}, {0.5, 1}}};

  s.x = 0;
  s.width = 40;

  EXPECT_EQ(s.outline(), (Points{{0.0, 20.0}, {40.0, 20.0}, {20.0, 60.0}}));
}

TEST(UTEST_ImageRecordSegment, outline_of_less_than_three_points_is_the_rect)
{
  const ImageRecordSegment s{"cat", 1, 2, 3, 4, {{0.0, 0.0}, {1.0, 1.0}}};

  EXPECT_EQ(s.outline(),
            (Points{{1.0, 2.0}, {4.0, 2.0}, {4.0, 6.0}, {1.0, 6.0}}));
}

TEST(UTEST_ImageRecordSegment, duplicate_shared_keeps_the_segment_type)
{
  const auto s = std::make_shared<ImageRecordSegment>(
      "cat", 1, 2, 3, 4, Points{{0, 0}, {1, 0}, {0, 1}});

  const auto dup = s->duplicate_shared();

  ASSERT_NE(std::dynamic_pointer_cast<ImageRecordSegment>(dup), nullptr);
  EXPECT_NE(dup.get(), s.get());
  EXPECT_TRUE(ImageRecordRect::equal(s, dup));
}

TEST(UTEST_ImageRecordSegment, segments_of_other_points_differ)
{
  const auto l = std::make_shared<ImageRecordSegment>(
      "cat", 1, 2, 3, 4, Points{{0, 0}, {1, 0}, {0, 1}});
  const auto r = std::make_shared<ImageRecordSegment>(
      "cat", 1, 2, 3, 4, Points{{0, 0}, {1, 1}, {0, 1}});

  EXPECT_FALSE(ImageRecordRect::equal(l, r));
}

TEST(UTEST_ImageRecordSegment, segment_never_equals_the_rect_of_its_box)
{
  const auto s = std::make_shared<ImageRecordSegment>(
      "cat", 1, 2, 3, 4, Points{{0, 0}, {1, 0}, {0, 1}});
  const auto r = std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4);

  EXPECT_FALSE(ImageRecordRect::equal(s, r));
  EXPECT_FALSE(ImageRecordRect::equal(r, s));
}
