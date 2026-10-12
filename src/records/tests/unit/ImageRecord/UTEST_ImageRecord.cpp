#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "ImageRecord.h"
#include "ImageRecordRect.h"

using namespace testing;
using ImagesAnnotatorDataDrivers013::ImageRecord;
using ImagesAnnotatorDataDrivers013::ImageRecordPtr;
using ImagesAnnotatorDataDrivers013::ImageRecordRect;
using ImagesAnnotatorDataDrivers013::ImageRecordRectPtr;
using ImagesAnnotatorDataDrivers013::ImageRecordsSet;

namespace
{

ImageRecordRectPtr make_rect(const std::string& name, const int x, const int y,
                             const int width, const int height)
{
  return std::make_shared<ImageRecordRect>(name, x, y, width, height);
}

/// @brief Creates a record holding a single, currently selected rectangle.
ImageRecordPtr make_selected(const std::string& path, const std::string& absdir,
                             const std::string& rectName)
{
  auto ir = ImageRecord::create(path, absdir);

  ir->rects.emplace_back(make_rect(rectName, 1, 2, 3, 4));
  ir->current_rect = ir->rects.back();

  return ir;
}

}  // namespace

TEST(UTEST_ImageRecord, default_constructed_record_is_empty)
{
  ImageRecord ir;

  EXPECT_TRUE(ir.path.empty());
  EXPECT_TRUE(ir.abs_dir_path.empty());
  EXPECT_TRUE(ir.tmppath.empty());
  EXPECT_TRUE(ir.rects.empty());
  EXPECT_EQ(ir.current_rect, nullptr);
  EXPECT_EQ(ir.iwidth, 0);
  EXPECT_EQ(ir.iheight, 0);
  // 0 is the "no user-selected scale yet" sentinel.
  EXPECT_DOUBLE_EQ(ir.imageScale, 0.0);
}

TEST(UTEST_ImageRecord, the_first_constructor_argument_is_the_relative_path)
{
  const ImageRecord ir{"rel.png", "/abs"};

  EXPECT_EQ(ir.path, "rel.png");
  EXPECT_EQ(ir.abs_dir_path, "/abs");
}

TEST(UTEST_ImageRecord, create_forwards_both_paths_and_argument_less_is_empty)
{
  const auto ir = ImageRecord::create("rel.png", "/abs");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "rel.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs");

  const auto empty = ImageRecord::create();

  ASSERT_NE(empty, nullptr);
  EXPECT_TRUE(empty->path.empty());
  EXPECT_TRUE(empty->abs_dir_path.empty());
}

TEST(UTEST_ImageRecord, get_full_path_joins_the_root_directory_with_the_path)
{
  EXPECT_EQ(ImageRecord::create("b.png", "/root")->get_full_path(),
            "/root/b.png");
}

TEST(UTEST_ImageRecord, get_full_path_does_not_double_the_separator)
{
  EXPECT_EQ(ImageRecord::create("b.png", "/root/")->get_full_path(),
            "/root/b.png");
}

TEST(UTEST_ImageRecord, get_full_path_keeps_a_relative_path_as_it_is)
{
  const auto ir = ImageRecord::create("sub/only.png", "");

  EXPECT_EQ(ir->get_full_path(), "sub/only.png");
}

TEST(UTEST_ImageRecord, get_full_path_of_a_totally_empty_record_is_empty)
{
  EXPECT_TRUE(ImageRecord::create()->get_full_path().empty());
}

TEST(UTEST_ImageRecord, get_full_path_prefers_the_tmp_cache_path)
{
  auto ir = ImageRecord::create("b.png", "/root");
  ir->tmppath = std::filesystem::path{"/tmp/cache/x.png"};

  EXPECT_EQ(ir->get_full_path(), "/tmp/cache/x.png");
}

TEST(UTEST_ImageRecord, get_full_path_uses_the_tmp_path_even_without_any_other)
{
  auto ir = ImageRecord::create();
  ir->tmppath = std::filesystem::path{"/tmp/cache/x.png"};

  EXPECT_EQ(ir->get_full_path(), "/tmp/cache/x.png");
}

TEST(UTEST_ImageRecord, scale_stepping_moves_the_scale_by_a_single_step)
{
  auto ir = ImageRecord::create();
  ir->imageScale = 1.0;

  ir->scaleStepIn();
  EXPECT_DOUBLE_EQ(ir->imageScale, 1.0 + ImageRecord::defaultScaleStep);

  ir->scaleStepOut();
  EXPECT_DOUBLE_EQ(ir->imageScale, 1.0);
}

TEST(UTEST_ImageRecord, scale_step_in_from_the_unset_sentinel_gives_one_step)
{
  auto ir = ImageRecord::create();

  ASSERT_DOUBLE_EQ(ir->imageScale, 0.0);

  ir->scaleStepIn();

  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::defaultScaleStep);
}

TEST(UTEST_ImageRecord, scale_step_out_from_the_unset_sentinel_gives_min_scale)
{
  auto ir = ImageRecord::create();

  ASSERT_DOUBLE_EQ(ir->imageScale, 0.0);

  // Stepping out of the 0 sentinel must not produce a zero or a negative
  // scale, the minimum takes over instead.
  ir->scaleStepOut();

  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::minScale);
  EXPECT_GT(ir->imageScale, 0.0);
}

TEST(UTEST_ImageRecord, scale_step_out_is_clamped_at_the_min_scale)
{
  auto ir = ImageRecord::create();
  ir->imageScale = ImageRecord::minScale;

  for (int i = 0; i < 100; ++i) {
    ir->scaleStepOut();
  }

  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::minScale);
  EXPECT_GT(ir->imageScale, 0.0);
}

TEST(UTEST_ImageRecord, scale_step_in_is_clamped_at_the_max_scale)
{
  auto ir = ImageRecord::create();
  ir->imageScale = ImageRecord::maxScale - ImageRecord::defaultScaleStep;

  ir->scaleStepIn();
  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::maxScale);

  for (int i = 0; i < 100; ++i) {
    ir->scaleStepIn();
  }

  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::maxScale);
}

TEST(UTEST_ImageRecord, erase_current_rect_without_a_selection_fails)
{
  auto ir = ImageRecord::create("a.png", "/d");
  ir->rects.emplace_back(make_rect("cat", 1, 2, 3, 4));

  EXPECT_FALSE(ir->erase_current_rect());
  EXPECT_EQ(ir->rects.size(), 1U);
}

TEST(UTEST_ImageRecord, erase_current_rect_of_a_foreign_rect_changes_nothing)
{
  auto ir = ImageRecord::create("a.png", "/d");
  ir->rects.emplace_back(make_rect("cat", 1, 2, 3, 4));

  // A value wise equal, but not owned, rectangle is still a foreign one:
  // the lookup goes by the very pointer.
  ir->current_rect = make_rect("cat", 1, 2, 3, 4);

  EXPECT_FALSE(ir->erase_current_rect());
  EXPECT_EQ(ir->rects.size(), 1U);
  EXPECT_NE(ir->current_rect, nullptr);
}

TEST(UTEST_ImageRecord, erase_current_rect_drops_the_rect_and_the_selection)
{
  auto ir = make_selected("a.png", "/d", "cat");

  ASSERT_EQ(ir->rects.size(), 1U);

  EXPECT_TRUE(ir->erase_current_rect());
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(ir->current_rect, nullptr);

  // The selection is gone, so a second call has nothing left to do.
  EXPECT_FALSE(ir->erase_current_rect());
}

TEST(UTEST_ImageRecord, erase_current_rect_keeps_the_other_rects_in_order)
{
  auto ir = ImageRecord::create("a.png", "/d");

  const auto first = make_rect("first", 1, 1, 1, 1);
  const auto middle = make_rect("same", 2, 2, 2, 2);
  const auto twin = make_rect("same", 2, 2, 2, 2);
  const auto last = make_rect("last", 3, 3, 3, 3);

  ir->rects = {first, middle, twin, last};
  ir->current_rect = twin;

  EXPECT_TRUE(ir->erase_current_rect());

  ASSERT_EQ(ir->rects.size(), 3U);
  EXPECT_EQ(ir->rects[0].get(), first.get());
  // The value wise identical twin stays: only the selected pointer went away.
  EXPECT_EQ(ir->rects[1].get(), middle.get());
  EXPECT_EQ(ir->rects[2].get(), last.get());
}

TEST(UTEST_ImageRecord, equal_ptr_of_the_very_same_record_is_true)
{
  const auto ir = make_selected("a.png", "/d", "cat");

  EXPECT_TRUE(ImageRecord::equal(ir, ir));
}

TEST(UTEST_ImageRecord, equal_ptr_compares_distinct_records_value_wise)
{
  auto l = ImageRecord::create("p", "/d");
  auto r = ImageRecord::create("p", "/d");

  l->iwidth = 640;
  l->iheight = 480;
  l->imageScale = 1.5;
  r->iwidth = 640;
  r->iheight = 480;
  r->imageScale = 1.5;

  EXPECT_TRUE(ImageRecord::equal(l, r));
}

TEST(UTEST_ImageRecord, equal_ptr_detects_each_differing_field)
{
  const auto base = ImageRecord::create("p", "/d");

  EXPECT_FALSE(ImageRecord::equal(base, ImageRecord::create("q", "/d")));
  EXPECT_FALSE(ImageRecord::equal(base, ImageRecord::create("p", "/other")));

  auto scaled = ImageRecord::create("p", "/d");
  scaled->imageScale = 2.0;
  EXPECT_FALSE(ImageRecord::equal(base, scaled));

  auto wider = ImageRecord::create("p", "/d");
  wider->iwidth = 1;
  EXPECT_FALSE(ImageRecord::equal(base, wider));

  auto higher = ImageRecord::create("p", "/d");
  higher->iheight = 1;
  EXPECT_FALSE(ImageRecord::equal(base, higher));
}

TEST(UTEST_ImageRecord, equal_ptr_takes_the_rectangles_into_account)
{
  const auto plain = ImageRecord::create("p", "/d");

  EXPECT_FALSE(ImageRecord::equal(make_selected("p", "/d", "cat"), plain));
  EXPECT_TRUE(ImageRecord::equal(make_selected("p", "/d", "cat"),
                                 make_selected("p", "/d", "cat")));
  EXPECT_FALSE(ImageRecord::equal(make_selected("p", "/d", "cat"),
                                  make_selected("p", "/d", "dog")));
}

TEST(UTEST_ImageRecord, equal_ptr_ignores_the_tmp_path_and_the_selection)
{
  auto l = make_selected("p", "/d", "cat");
  auto r = make_selected("p", "/d", "cat");

  l->tmppath = std::filesystem::path{"/tmp/cache/l.png"};
  r->current_rect.reset();

  EXPECT_TRUE(ImageRecord::equal(l, r));
}

TEST(UTEST_ImageRecord, equal_set_short_circuits_on_the_same_set_reference)
{
  ImageRecordsSet single{ImageRecord::create("a.png", "/d")};

  EXPECT_TRUE(ImageRecord::equal(single, single));
}

TEST(UTEST_ImageRecord, equal_set_of_two_empty_sets_is_true)
{
  const ImageRecordsSet l;
  const ImageRecordsSet r;

  EXPECT_TRUE(ImageRecord::equal(l, r));
}

TEST(UTEST_ImageRecord, equal_set_compares_the_records_value_wise)
{
  const ImageRecordsSet l{make_selected("a.png", "/d", "cat"),
                          ImageRecord::create("b.png", "/d")};
  const ImageRecordsSet r{make_selected("a.png", "/d", "cat"),
                          ImageRecord::create("b.png", "/d")};

  EXPECT_TRUE(ImageRecord::equal(l, r));

  const ImageRecordsSet differing{make_selected("a.png", "/d", "cat"),
                                  ImageRecord::create("c.png", "/d")};

  EXPECT_FALSE(ImageRecord::equal(l, differing));
}

TEST(UTEST_ImageRecord, equal_set_is_order_sensitive)
{
  const auto a = ImageRecord::create("a.png", "/d");
  const auto b = ImageRecord::create("b.png", "/d");

  const ImageRecordsSet straight{a, b};
  const ImageRecordsSet reversed{b, a};

  EXPECT_FALSE(ImageRecord::equal(straight, reversed));
}

TEST(UTEST_ImageRecord, equal_set_of_different_sizes_is_false)
{
  const ImageRecordsSet longer{ImageRecord::create("a.png", "/d"),
                               ImageRecord::create("b.png", "/d")};
  const ImageRecordsSet shorter{ImageRecord::create("a.png", "/d")};

  EXPECT_FALSE(ImageRecord::equal(longer, shorter));
  EXPECT_FALSE(ImageRecord::equal(shorter, longer));
  EXPECT_FALSE(ImageRecord::equal(shorter, ImageRecordsSet{}));
}

TEST(UTEST_ImageRecord, duplicate_shared_is_a_distinct_but_equal_record)
{
  auto orig = make_selected("p", "/d", "cat");
  orig->imageScale = 1.5;
  orig->iwidth = 10;
  orig->iheight = 20;

  const auto dup = orig->duplicate_shared();

  ASSERT_NE(dup, nullptr);
  EXPECT_NE(dup.get(), orig.get());
  EXPECT_TRUE(ImageRecord::equal(orig, dup));
  EXPECT_EQ(dup->path, "p");
  EXPECT_EQ(dup->abs_dir_path, "/d");
  EXPECT_DOUBLE_EQ(dup->imageScale, 1.5);
  EXPECT_EQ(dup->iwidth, 10);
  EXPECT_EQ(dup->iheight, 20);
}

TEST(UTEST_ImageRecord, duplicate_shared_deep_copies_every_rectangle)
{
  auto orig = make_selected("p", "/d", "cat");
  orig->rects.emplace_back(make_rect("dog", 5, 6, 7, 8));

  const auto dup = orig->duplicate_shared();

  ASSERT_EQ(dup->rects.size(), orig->rects.size());

  for (std::size_t i = 0; i < orig->rects.size(); ++i) {
    EXPECT_NE(dup->rects[i].get(), orig->rects[i].get());
    EXPECT_TRUE(ImageRecordRect::equal(orig->rects[i], dup->rects[i]));
  }

  dup->rects[0]->name = "changed";

  EXPECT_EQ(orig->rects[0]->name, "cat");
  EXPECT_FALSE(ImageRecord::equal(orig, dup));
}

TEST(UTEST_ImageRecord, duplicate_shared_leaves_the_tmp_path_and_selection_out)
{
  auto orig = make_selected("p", "/d", "cat");
  orig->tmppath = std::filesystem::path{"/tmp/cache/p.png"};

  const auto dup = orig->duplicate_shared();

  EXPECT_TRUE(dup->tmppath.empty());
  EXPECT_EQ(dup->current_rect, nullptr);
  EXPECT_EQ(dup->get_full_path(), "/d/p");
}

TEST(UTEST_ImageRecord, duplicate_deep_copies_the_whole_set)
{
  const ImageRecordsSet orig{make_selected("a.png", "/d", "cat"),
                             ImageRecord::create("b.png", "/d")};

  const auto dup = ImageRecord::duplicate(orig);

  ASSERT_EQ(dup.size(), orig.size());
  EXPECT_TRUE(ImageRecord::equal(orig, dup));

  for (std::size_t i = 0; i < orig.size(); ++i) {
    EXPECT_NE(dup[i].get(), orig[i].get());
  }

  ASSERT_FALSE(dup[0]->rects.empty());
  EXPECT_NE(dup[0]->rects[0].get(), orig[0]->rects[0].get());
}

TEST(UTEST_ImageRecord, duplicate_of_an_empty_set_gives_an_empty_set)
{
  EXPECT_TRUE(ImageRecord::duplicate(ImageRecordsSet{}).empty());
}
