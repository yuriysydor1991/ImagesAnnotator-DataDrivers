## The imgajz2 project format of the segments

The imgajz2 file (`*.imgajz2`, the **im**a**g**es **a**nnotator **j**son **z**ipped file of the second version) is the third project file format of the library and the only one keeping the segments - the `ImageRecordSegment` polygon annotations: the gzip compressed JSON project file the way the [imgajz1 file](/doc/sections/en_US/4-project-structure/4-11-the-imgajz1-compressed-project-format.md) is, which keeps the polygon of every segment under the `"segment"` key next to its `"rect"` as well and declares its format at the root. The rest of its document is the one of the [annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md).

### The segments in the document

```
root object
  "<app name>": { "version": "<app version>" }   the identity element
  "annotations": [ ... ]                         the source folders of the JSON format
  "format": { "name": "imgajz2",                 the format declaration
              "types": [ "rect", "segment" ] }

the "annotations" array of an image record
  { "name": "dog",                               a rectangle
    "rect": { "x": 50, "y": 20, "width": 100, "height": 40 } },
  { "name": "cat",                               a segment: its rectangle and its polygon
    "rect": { "x": 10, "y": 20, "width": 20, "height": 40 },
    "segment": [ [0.0, 0.0], [1.0, 0.0], [0.5, 1.0] ] }
```

The `"segment"` value is the array of the `[x, y]` points of the polygon in the 0..1 space of the `"rect"`: the x is the share of the rect width from its left edge, the y the share of its height from its top one. So the `[0.5, 1.0]` point of the `cat` segment is the `(20, 60)` pixel of the image, the other two its `(10, 20)` and `(30, 20)` pixels, the rect is the bounding box of the polygon, and a rect moved or resized by an application carries the polygon along. An annotation of no `"segment"` key is a rectangle, the way the JSON project file keeps it.

The `"format"` declaration tells another application the file may hold segments. Every imgajz2 file the library writes carries it, the one of no segment too, while the reader of the library needs none: it reads the imgajz1 and the JSON project content as well, the project of the rectangles.

### Which format keeps the segments

The file name extension of the path tells the format, for the load and for the store alike:

| The path | The format | The annotations it keeps |
| --- | --- | --- |
| `*.imgajz2` (of any letter case: `*.IMGAJZ2` too) | the imgajz2 project file | the rectangles and the segments |
| `*.imgajz1` (of any letter case) | the imgajz1 project file | the rectangles alone |
| anything else | the JSON project file | the rectangles alone |

So the store of a database holding a segment into a `.json` or a `.imgajz1` path fails - `store_db()` returns `false`, logs the error and leaves the file as it was - rather than write the rectangle of the segment alone and lose its polygon: a project of segments goes on in the imgajz2 files only. The JSON and the imgajz1 readers, the other way round, read an annotation of the `"segment"` key - of an imgajz2 document under their name - as the rectangle of its `"rect"`, ignoring the key.

```cpp
#include <ImagesAnnotatorDataDrivers-0.15/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers015;

auto db = iadd::IADataDriversFacade::open_annotations_db("cars.imgajz1");

if (db == nullptr || db->get_images_db().empty()) {
  return false;
}

auto ir = db->get_images_db().front();
auto segment = iadd::IADataDriversFacade::create_records_factory()
                   ->create_image_segment_record(ir);

// the polygon of the image pixels, which the rectangle becomes the bounding
// box of
segment->name = "cat";
segment->add_point(10.0, 20.0);
segment->add_point(30.0, 20.0);
segment->add_point(20.0, 60.0);

ir->rects.emplace_back(segment);

// the cars.imgajz1 file keeps no segments, so the database goes on with the
// cars.imgajz2 file from here
return db->store_db("cars.imgajz2");
```

A library older than the format takes the `.imgajz2` path for the plain JSON project file and fails on its gzip data, so an application built against it refuses the imgajz2 file rather than open its segments as the rectangles and drop the polygons with the next store.

### The stock tools

The imgajz2 file is a plain gzip file of the JSON text as well, so `gzip -dc` prints its document for every JSON tool - though the JSON project file of that very text is a project of the rectangles alone to the library:

```
# the format declaration of the imgajz2 project file
gzip -dc cars.imgajz2 | jq '.format'

# the class names of the segments of the project
gzip -dc cars.imgajz2 | jq -r '.annotations[].annotations[].annotations[] | select(has("segment")) | .name' | sort -u
```

### Reading and writing

The file is written and read the way the imgajz1 one is, with two differences: the document the writer prints keeps the `"segment"` points of every segment and the `"format"` declaration, and the reader builds an annotation of the `"segment"` key through the `create_image_segment_record()` of the records factory, the rest through its `create_image_rect_record()` - so a custom factory builds the segments as well.

### The implementation

The format is the [Imgajz2LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz2LibraryImpl.h), the [Imgajz1LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h) descendant the `AnnotationsDirDB` performs the load and the store of the `.imgajz2` paths through (its `impl_of()`), configured with the application identity and the records factory of the `LibraryImpl` the database keeps. It keeps the gzip compressed `read_text()` and `write_text()` of its base and overrides the two protected virtual converter factory methods of the [LibraryImpl](/src/dbs/AnnotationsDBs/LibraryImpl.h): the `create_writer()` of the [ImageRecordSegments2JSONConverter](/src/dbs/AnnotationsDBs/converters/ImageRecordSegments2JSONConverter.h) - the `ImageRecords2JSONConverter` descendant writing the `"segment"` and the `"format"` keys - and the `create_rect_reader()` of the [ImageRecordSegmentJSON2RecordConverter](/src/dbs/AnnotationsDBs/converters/ImageRecordSegmentJSON2RecordConverter.h) - the `ImageRecordRectJSON2RecordConverter` descendant reading the segments. The `ImageRecords2JSONConverter` the JSON and the imgajz1 files are written with throws the `std::invalid_argument` for a segment, which fails their store before the file is opened.
