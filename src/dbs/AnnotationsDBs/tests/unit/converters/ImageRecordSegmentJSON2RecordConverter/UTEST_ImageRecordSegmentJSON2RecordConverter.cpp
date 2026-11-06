#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <nlohmann/json.hpp>

#include "ImageRecordRect.h"
#include "ImageRecordSegment.h"
#include "src/dbs/AnnotationsDBs/converters/ImageRecordSegmentJSON2RecordConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::
    ImageRecordSegmentJSON2RecordConverter;
using ImagesAnnotatorDataDrivers015::ImageRecordRect;
using ImagesAnnotatorDataDrivers015::ImageRecordSegment;

namespace
{
const auto rect = nlohmann::json::parse(
    R"({"name": "cat", "rect": {"x": 5, "y": 6, "width": 7, "height": 8}})");
}

TEST(UTEST_ImageRecordSegmentJSON2RecordConverter, reads_a_segment)
{
  auto j = rect;

  j["segment"] = nlohmann::json::parse("[[0, 0], [1, 0], [0.5, 1]]");

  const auto r = ImageRecordSegmentJSON2RecordConverter{}.convert(j);

  EXPECT_TRUE(ImageRecordRect::equal(
      r, std::make_shared<ImageRecordSegment>(
             "cat", 5, 6, 7, 8,
             ImageRecordRect::Points{{0, 0}, {1, 0}, {0.5, 1}})));
}

TEST(UTEST_ImageRecordSegmentJSON2RecordConverter, reads_a_rect_of_no_segment)
{
  const auto r = ImageRecordSegmentJSON2RecordConverter{}.convert(rect);

  EXPECT_TRUE(ImageRecordRect::equal(
      r, std::make_shared<ImageRecordRect>("cat", 5, 6, 7, 8)));
}

TEST(UTEST_ImageRecordSegmentJSON2RecordConverter, refuses_malformed_points)
{
  auto j = rect;

  j["segment"] = "none";

  EXPECT_EQ(ImageRecordSegmentJSON2RecordConverter{}.convert(j), nullptr);
}
