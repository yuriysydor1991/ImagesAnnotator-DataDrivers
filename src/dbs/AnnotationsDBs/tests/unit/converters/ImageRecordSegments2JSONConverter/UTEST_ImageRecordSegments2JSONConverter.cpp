#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <nlohmann/json.hpp>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "ImageRecordSegment.h"
#include "src/dbs/AnnotationsDBs/converters/ImageRecordSegments2JSONConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::
    ImageRecordSegments2JSONConverter;
using ImagesAnnotatorDataDrivers015::ImageRecord;
using ImagesAnnotatorDataDrivers015::ImageRecordRect;
using ImagesAnnotatorDataDrivers015::ImageRecordSegment;

TEST(UTEST_ImageRecordSegments2JSONConverter, keeps_the_segment_points)
{
  auto ir = std::make_shared<ImageRecord>("a.png", "/images");

  ir->rects.emplace_back(std::make_shared<ImageRecordRect>("dog", 1, 2, 3, 4));
  ir->rects.emplace_back(std::make_shared<ImageRecordSegment>(
      "cat", 5, 6, 7, 8, ImageRecordRect::Points{{0, 0}, {1, 0}, {0.5, 1}}));

  const auto j =
      ImageRecordSegments2JSONConverter{"App", "1.0"}.to_document({ir});
  const auto& rects = j["annotations"][0]["annotations"][0]["annotations"];

  EXPECT_EQ(rects[0], nlohmann::json::parse(
                          R"({"name": "dog", "rect": {"x": 1, "y": 2,
                              "width": 3, "height": 4}})"));
  EXPECT_EQ(rects[1]["rect"]["x"], 5);
  EXPECT_EQ(rects[1]["segment"],
            nlohmann::json::parse("[[0.0, 0.0], [1.0, 0.0], [0.5, 1.0]]"));
  EXPECT_EQ(j["format"]["name"], "imgajz2");
  EXPECT_EQ(j["App"]["version"], "1.0");
}
