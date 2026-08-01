#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::ImageRecords2JSONConverter;
using ImagesAnnotatorDataDrivers011::ImageRecord;
using ImagesAnnotatorDataDrivers011::ImageRecordRect;
using ImagesAnnotatorDataDrivers011::ImageRecordRectSet;
using ImagesAnnotatorDataDrivers011::ImageRecordsSet;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace
{

// Exposes the protected convert() overloads for focused unit testing.
class TestableConverter : public ImageRecords2JSONConverter
{
 public:
  using ImageRecords2JSONConverter::convert;
};

nlohmann::json read_json(const std::filesystem::path& from)
{
  std::ifstream file(from.string().c_str());

  if (!file.is_open()) {
    return nlohmann::json{};
  }

  nlohmann::json parsed;

  file >> parsed;

  return parsed;
}

}  // namespace

TEST(UTEST_ImageRecords2JSONConverter, converts_a_rect_set_to_json)
{
  TestableConverter conv;
  ImageRecordRectSet rects{
      std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4)};

  auto j = conv.convert(rects);

  ASSERT_TRUE(j.is_array());
  ASSERT_EQ(j.size(), 1U);
  EXPECT_EQ(j[0][S::annotation_rect_name].get<std::string>(), "cat");
  EXPECT_EQ(j[0][S::annotation_rect][S::annotation_rect_x].get<int>(), 1);
  EXPECT_EQ(j[0][S::annotation_rect][S::annotation_rect_y].get<int>(), 2);
  EXPECT_EQ(j[0][S::annotation_rect][S::annotation_rect_width].get<int>(), 3);
  EXPECT_EQ(j[0][S::annotation_rect][S::annotation_rect_height].get<int>(), 4);
}

TEST(UTEST_ImageRecords2JSONConverter, converts_an_image_record_to_json)
{
  TestableConverter conv;

  auto ir = ImageRecord::create("rel.png", "/abs");
  ir->imageScale = 1.5;
  ir->iwidth = 10;
  ir->iheight = 20;
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4));

  auto j = conv.convert(ir);

  EXPECT_EQ(j[S::annotation_recerod_rel_path_field].get<std::string>(),
            "rel.png");
  EXPECT_DOUBLE_EQ(j[S::annotation_image_scale_factor].get<double>(), 1.5);
  EXPECT_EQ(j[S::annotation_image_width].get<int>(), 10);
  EXPECT_EQ(j[S::annotation_image_height].get<int>(), 20);
  ASSERT_TRUE(j[S::annotations_field_name].is_array());
  EXPECT_EQ(j[S::annotations_field_name].size(), 1U);
}

TEST(UTEST_ImageRecords2JSONConverter, groups_records_by_absolute_directory)
{
  TestableConverter conv;

  ImageRecordsSet irs{ImageRecord::create("a.png", "/d1"),
                      ImageRecord::create("b.png", "/d1")};

  auto j = conv.convert(irs);

  ASSERT_TRUE(j.is_array());
  ASSERT_EQ(j.size(), 1U);
  EXPECT_EQ(j[0][S::annotation_db_field_name][S::annotations_db_path_field_name]
                .get<std::string>(),
            "/d1");
  EXPECT_EQ(j[0][S::annotations_field_name].size(), 2U);
}

TEST(UTEST_ImageRecords2JSONConverter, stores_the_configured_app_identity)
{
  const std::string customName{"MyOwnAnnotator"};
  const std::string customVersion{"9.9.9-custom"};

  const std::filesystem::path tmpFile =
      std::filesystem::temp_directory_path() /
      "UTEST_ImageRecords2JSONConverter_identity.json";

  std::filesystem::remove(tmpFile);

  const ImageRecordsSet irs{ImageRecord::create("a.png", "/d1")};

  ImageRecords2JSONConverter customized{customName, customVersion};

  EXPECT_EQ(customized.app_name, customName);
  EXPECT_EQ(customized.app_version, customVersion);

  ASSERT_TRUE(customized.store(irs, tmpFile.string()));

  const auto customJson = read_json(tmpFile);

  ASSERT_TRUE(customJson.contains(customName));
  EXPECT_EQ(
      customJson[customName][S::file_version_field_name].get<std::string>(),
      customVersion);
  EXPECT_FALSE(customJson.contains("ImagesAnnotator"));

  ImageRecords2JSONConverter defaulted;

  EXPECT_EQ(defaulted.app_name, "ImagesAnnotator");
  EXPECT_EQ(defaulted.app_version, "0.11.0");

  ASSERT_TRUE(defaulted.store(irs, tmpFile.string()));

  const auto defaultJson = read_json(tmpFile);

  ASSERT_TRUE(defaultJson.contains("ImagesAnnotator"));
  EXPECT_EQ(defaultJson["ImagesAnnotator"][S::file_version_field_name]
                .get<std::string>(),
            "0.11.0");
  EXPECT_FALSE(defaultJson.contains(customName));

  EXPECT_TRUE(std::filesystem::remove(tmpFile));
}
