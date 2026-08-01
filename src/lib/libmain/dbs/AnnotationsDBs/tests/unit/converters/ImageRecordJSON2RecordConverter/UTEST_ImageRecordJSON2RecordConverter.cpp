#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "IRecordsFactory.h"
#include "ImageRecord.h"
#include "ImageRecordRect.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/lib/libmain/dbs/AnnotationsDBs/converters/ImageRecordJSON2RecordConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::ImageRecordJSON2RecordConverter;
using ImagesAnnotatorDataDrivers011::ImageRecord;
using ImagesAnnotatorDataDrivers011::ImageRecordPtr;
using ImagesAnnotatorDataDrivers011::ImageRecordRectPtr;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace
{

// The caller supplied records factory stand-in.
class RecordsFactoryMock : public ImagesAnnotatorDataDrivers011::IRecordsFactory
{
 public:
  MOCK_METHOD(ImageRecordPtr, create_image_record,
              (const std::string& nirpath, const std::string& nabsPath),
              (override));
  MOCK_METHOD(ImageRecordRectPtr, create_image_rect_record,
              (const ImageRecordPtr& ir), (override));
  MOCK_METHOD(ImageRecordRectPtr, create_image_rect_record,
              (const std::string& nname, const int& nx, const int& ny,
               const int& nwidth, const int& nheight),
              (override));
};

nlohmann::json rect_json(const std::string& name, int x, int y, int w, int h)
{
  nlohmann::json rect;
  rect[S::annotation_rect_name] = name;
  rect[S::annotation_rect][S::annotation_rect_x] = x;
  rect[S::annotation_rect][S::annotation_rect_y] = y;
  rect[S::annotation_rect][S::annotation_rect_width] = w;
  rect[S::annotation_rect][S::annotation_rect_height] = h;
  return rect;
}

nlohmann::json annotation_json(const std::string& relPath)
{
  nlohmann::json fan;
  fan[S::annotation_recerod_rel_path_field] = relPath;
  fan[S::annotation_image_scale_factor] = 1.5;
  fan[S::annotation_image_width] = 10;
  fan[S::annotation_image_height] = 20;
  fan[S::annotations_field_name] = nlohmann::json::array();
  return fan;
}

}  // namespace

TEST(UTEST_ImageRecordJSON2RecordConverter, converts_record_metadata)
{
  ImageRecordJSON2RecordConverter conv;

  auto ir = conv.convert(annotation_json("img.png"), "/abs/dir");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "img.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs/dir");
  EXPECT_DOUBLE_EQ(ir->imageScale, 1.5);
  EXPECT_EQ(ir->iwidth, 10);
  EXPECT_EQ(ir->iheight, 20);
  EXPECT_TRUE(ir->rects.empty());
}

TEST(UTEST_ImageRecordJSON2RecordConverter, converts_nested_rectangles)
{
  ImageRecordJSON2RecordConverter conv;

  auto fan = annotation_json("img.png");
  fan[S::annotations_field_name].push_back(rect_json("cat", 1, 2, 3, 4));
  fan[S::annotations_field_name].push_back(rect_json("dog", 5, 6, 7, 8));

  auto ir = conv.convert(fan, "/abs/dir");

  ASSERT_NE(ir, nullptr);
  ASSERT_EQ(ir->rects.size(), 2U);
  EXPECT_EQ(ir->rects[0]->name, "cat");
  EXPECT_EQ(ir->rects[0]->x, 1);
  EXPECT_EQ(ir->rects[1]->name, "dog");
  EXPECT_EQ(ir->rects[1]->height, 8);
}

TEST(UTEST_ImageRecordJSON2RecordConverter, uses_the_caller_supplied_factory)
{
  auto factory = std::make_shared<StrictMock<RecordsFactoryMock>>();

  const auto produced = ImageRecord::create("img.png", "/abs/dir");

  EXPECT_CALL(*factory, create_image_record(std::string{"img.png"},
                                            std::string{"/abs/dir"}))
      .Times(1)
      .WillOnce(Return(produced));

  ImageRecordJSON2RecordConverter conv{factory};

  auto ir = conv.convert(annotation_json("img.png"), "/abs/dir");

  EXPECT_EQ(ir, produced);
  EXPECT_TRUE(Mock::VerifyAndClearExpectations(factory.get()));
}
