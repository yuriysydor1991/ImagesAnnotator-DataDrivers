## The annotations project JSON format

An annotations project is a single JSON document. This section describes it as the library actually implements it - the field names come from [AnnotationsJSONSerializator.h](/src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h), the reading rules from the converters under [src/dbs/AnnotationsDBs/converters](/src/dbs/AnnotationsDBs/converters), and the validation rules from [AnnotationsJSONSerializator.cpp](/src/dbs/AnnotationsDBs/AnnotationsJSONSerializator.cpp).

Nothing here is a private detail: the format is the whole point of the library, and a file it writes must stay readable by the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application and by anybody else's tooling.

### The shape in one glance

```
root object
  "<app name>": { "version": "<app version>" }   the identity element
  "annotations": [                               the source folders
    { "db": { "path": "<absolute dir path or URL>" },
      "annotations": [                           the image records of that folder
        { "rel_path": "<path relative to db.path>",
          "scale_factor": <double>,
          "width":  <int>,
          "height": <int>,
          "annotations": [                       the rectangles of that image
            { "name": "<class name>",
              "rect": { "x": <int>, "y": <int>,
                        "width": <int>, "height": <int> } } ] } ] } ]
```

Three levels, and the key `"annotations"` is reused at every one of them - once as the array of source folders, once as the array of images inside a folder, once as the array of rectangles inside an image. The nesting is what tells them apart.

There is a second reuse to be aware of: `"width"` and `"height"` mean the intrinsic **image** size when they sit next to `"rel_path"`, and the **rectangle** size when they sit inside `"rect"`. Both spellings are literally the same constants in the code.

### The root object

| Field | Type | Required | Meaning |
| --- | --- | --- | --- |
| `<app name>` | object | **yes** | the identity element, named after the *application*. `"ImagesAnnotator"` by default |
| `<app name>.version` | string | **yes** | the version of the application that wrote the file |
| `annotations` | array | **yes** | the source folder groups |

The root element name is not the library name and not a fixed literal - it is whatever the database instance is configured with. That is a compatibility decision, and the [configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md) section explains it in full. The short version: files written by ImagesAnnotator carry an `"ImagesAnnotator"` root, so the library defaults to that name and lets you override it at the build time or at the run time.

A reader looks the root element up **by the name it is configured with**. Read an `"ImagesAnnotator"` rooted file with a database configured for `"MyTool"` and it fails, not because the document is malformed but because it belongs to someone else.

### A source folder group

| Field | Type | Required | Meaning |
| --- | --- | --- | --- |
| `db` | object | **yes** | the source description |
| `db.path` | string | **yes**, non-empty | the absolute directory path, or the web page URL, the images were taken from |
| `annotations` | array | **yes** | the image records belonging to that source |

The grouping is not cosmetic: `db.path` becomes the `abs_dir_path` of every record inside the group, and it is the half of the image location that is not repeated per record. On writing, the records are re-grouped by their `abs_dir_path` and the groups come out ordered by that path.

### An image record

| Field | Type | Required | Maps to | Meaning |
| --- | --- | --- | --- | --- |
| `rel_path` | string | **yes** | `ImageRecord::path` | the image path relative to the enclosing `db.path` |
| `scale_factor` | double | optional | `ImageRecord::imageScale` | the preview zoom. Absent means `0` - "no user-selected scale yet" |
| `width` | int | optional | `ImageRecord::iwidth` | the intrinsic image width. Absent means `0` |
| `height` | int | optional | `ImageRecord::iheight` | the intrinsic image height. Absent means `0` |
| `annotations` | array | **yes** | `ImageRecord::rects` | the rectangles drawn over this image |

The three optional fields are genuinely optional on reading - each is taken only when the key is present, so a hand written file may leave all of them out. The writer, on the other hand, always emits them, so a file that went through a store cycle carries them all even when they are zero.

`ImageRecord::tmppath` and `ImageRecord::current_rect` never appear in the document. The first is a local network cache path and the second is a selection, and neither means anything to another machine or another session.

### A rectangle

| Field | Type | Required | Maps to |
| --- | --- | --- | --- |
| `name` | string | **yes** | `ImageRecordRect::name` - the annotation (class) name |
| `rect` | object | **yes** | the geometry |
| `rect.x` | int | **yes** | `ImageRecordRect::x` |
| `rect.y` | int | **yes** | `ImageRecordRect::y` |
| `rect.width` | int | **yes** | `ImageRecordRect::width` |
| `rect.height` | int | **yes** | `ImageRecordRect::height` |

The coordinates are in the image own pixel space, never in the scaled preview space, so `scale_factor` has no influence whatsoever on how a rectangle is to be interpreted.

### The worked example

The file below is the real fixture the `CTEST_AnnotationsDirDB` component test drives the library with, [src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/ctest_data/basic-structure-plan.json](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/ctest_data/basic-structure-plan.json). It loads successfully and yields **five** image records - three from the first folder and one from each of the other two:

```json
{
  "ImagesAnnotator": {
    "version": "0.10.0"
  },
  "user": {
    "time-spent": {
      "value": 1234,
      "dimension": "seconds" 
    }
  },
  "annotations": [
    {
      "db": {
        "path": "/absolute/path/to/images/db/dir/1"
      },
      "annotations": [
        {
          "rel_path": "rel/path/to/image/1.img",
          "annotations": [
            {
              "name": "annotation-tag-or-class-name-1",
              "rect": {
                "x": 123, "y": 456, "width": 123, "height": 456
              },
              "scale_factor": 1.55
            },
            {
              "name": "annotation-tag-or-class-name-2",
              "rect": {
                "x": 654, "y": 321, "width": 654, "height": 321
              },
              "scale_factor": 0.55
            }
          ]
        },
        {
          "rel_path": "rel/path/to/image/2.img",
          "annotations": [
            {
              "name": "annotation-tag-or-class-name-3",
              "rect": {
                "x": 987, "y": 876, "width": 987, "height": 876
              },
              "scale_factor": 0.65
            },
            {
              "name": "annotation-tag-or-class-name-1",
              "rect": {
                "x": 567, "y": 890, "width": 123, "height": 456
              },
              "scale_factor": 0.95
            }
          ]
        },
        {
          "rel_path": "rel/path/to/image/3.img",
          "annotations": [
            {
              "name": "annotation-tag-or-class-name-2",
              "rect": {
                "x": 321, "y": 654, "width": 987, "height": 654
              },
              "scale_factor": 0.75
            },
            {
              "name": "annotation-tag-or-class-name-4",
              "rect": {
                "x": 987, "y": 654, "width": 321, "height": 456
              },
              "scale_factor": 2.55
            }
          ]
        }
      ]
    },
    {
      "db": {
        "path": "/absolute/path/to/images/db/dir/2"
      },
      "annotations": [
        {
          "rel_path": "rel/path/to/image/1.img",
          "annotations": [
            {
              "name": "annotation-tag-or-class-name-3",
              "rect": {
                "x": 123, "y": 456, "width": 123, "height": 456
              },
              "scale_factor": 0.25
            },
            {
              "name": "annotation-tag-or-class-name-4",
              "rect": {
                "x": 654, "y": 321, "width": 654, "height": 321
              },
              "scale_factor": 0.55
            }
          ]
        }
      ]
    },
    {
      "db": {
        "path": "/absolute/path/to/images/db/dir/3"
      },
      "annotations": [
        {
          "rel_path": "rel/path/to/image/1.img",
          "annotations": [
            {
              "name": "annotation-tag-or-class-name-1",
              "rect": {
                "x": 123, "y": 456, "width": 123, "height": 456
              },
              "scale_factor": 0.55
            },
            {
              "name": "annotation-tag-or-class-name-4",
              "rect": {
                "x": 654, "y": 321, "width": 654, "height": 321
              },
              "scale_factor": 0.55
            }
          ]
        }
      ]
    }
  ]
}
```

What is worth pointing at in it:

- **The identity element says `0.10.0` while the library default is `0.11.0`.** The file still loads. The mismatch is a warning, see below.
- **The top level `"user"` object is not a part of the format.** The library ignores it entirely, which is exactly the point of having it in the fixture: an unknown key must never break a load. Be aware though that it does not survive a store either - see the round-trip note.
- **Every image record here omits `scale_factor`, `width` and `height`** at its own level. All three are optional, and the records come out with `imageScale`, `iwidth` and `iheight` at `0`.
- **The `"scale_factor"` keys inside the rectangle objects are ignored.** A rectangle is read for its `name` and its `rect` only; the image scale is an image record level field. The fixture carries them precisely because a real world file may hold junk in a place the format does not define one.
- **Nothing requires the records to be sorted in the document.** Sorting by the full path happens on merging, not on loading: a load hands the records over in exactly the order the file lists them.

### The version mismatch: a warning, not a refusal

When the identity element version differs from the version the database is configured with, the library logs a warning and **carries on loading**:

```
file's ImagesAnnotator app version is: 0.10.0
Current app version and files's ones missmatch: 0.10.0 != 0.11.0
```

That is deliberate. The format has been stable, the version is an informational stamp, and refusing a file written by a slightly older or slightly newer application would be a much worse failure mode than a log line. Do not build your own logic on top of the version string expecting the library to enforce anything about it.

### What makes a document invalid

`load_db()` returns `false` - and `LibraryFacade::open_annotations_db()` returns a `nullptr` - in any of these cases:

| Cause | Detail |
| --- | --- |
| the path is empty, or the file cannot be opened | logged with the `strerror` reason |
| the file is not parseable JSON | the parser exception is caught and logged |
| **the root identity element is missing** | looked up by the *configured* application name |
| the top level `annotations` array is missing | |
| a folder group has no `db` element | |
| a folder group has no `annotations` element | |
| a `db` element has no `path` | |
| **a `db.path` is present but empty** | an empty source root is not resolvable to anything |

The last four are checked for every folder group, and the first group that fails aborts the whole load - a partial database is never handed back.

The smallest possible invalid document is therefore `{}`, which is precisely what the second component test fixture [non_empty_annotation_db_init_serialize_failure.json](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/ctest_data/non_empty_annotation_db_init_serialize_failure.json) holds: valid JSON, no root element, load refused.

Note that a failed load does not roll the database back to what it held before - `load_db()` is a replacing operation and a failure leaves it in an unspecified state. Treat a `false` from it as "this instance is done", and build a fresh one.

Below that hard validation there is a softer layer. A folder group whose `db.path` is fine but whose individual image records are malformed - a record without `rel_path`, a rectangle without `rect` - is not something the format tolerates: the converters assert on those fields, so a debug build stops on the spot and a release build is in undefined territory. In other words: the required fields of the two innermost levels are required, and a producer must emit them.

### The round trip

Storing is not "editing the file you loaded", it is writing a fresh document out of the in-memory records. The consequences:

- keys the format does not define - like the fixture `"user"` object - are **not preserved**;
- the folder groups are rebuilt by grouping the records by their `abs_dir_path`, and they come out ordered by that path;
- inside a group the records keep the order they hold in memory;
- every optional image record field is written out, zero or not;
- the output is pretty printed with a two space indentation;
- the identity element is written with the name and the version the database is configured with **at the moment of the store**, which is not necessarily the ones the loaded file carried.

That last point is the one that bites: load a `0.10.0` file, store it with the defaults, and the stored one says `0.11.0`. Call `set_app_info()` with what you read back from `get_app_name()` / `get_app_version()` if preserving the original stamp matters to you.

### See also

- [The annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md) - the calls that read and write all of the above.
- [Configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md) - the root element name and the version stamp.
- The dataset export formats (plain text, YOLO v4, PyTorch-Vision) are **not** a part of this library and are not described here: they belong to a separate project of their own.
