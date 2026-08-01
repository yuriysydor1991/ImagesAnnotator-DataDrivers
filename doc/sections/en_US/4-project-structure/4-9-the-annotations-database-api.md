## The annotations database API

Everything a consuming project is allowed to touch lives in the [src/lib/facade/public](/src/lib/facade/public) directory, and every single one of those headers is installed. There are no other entry points: the implementation classes (`AnnotationsDirDB`, `RecordsFactory`, `DefaultDBMerger` and the rest of [src/lib/libmain](/src/lib/libmain)) stay behind the abstract interfaces and never appear in a downstream translation unit.

This section is a guided tour of that surface. Read it with the real headers open - they carry the doxygen comments this text is derived from.

### The versioned namespace

Every public type sits in the `ImagesAnnotatorDataDrivers011` namespace. The `011` suffix is the library major and the minor version glued together, and it is there on purpose: two different library versions may be pulled into one and the same translation unit without a single symbol clash.

Writing that name out in full everywhere would be painful, so alias it once and be done with it:

```cpp
namespace iadd = ImagesAnnotatorDataDrivers011;
```

A library version bump then stays a one line change on your side. The rest of this section uses the `iadd` alias.

### The header set

| Header | What it gives you |
| --- | --- |
| [LibraryFacade.h](/src/lib/facade/public/LibraryFacade.h) | the static factory front door - the only header a simple consumer needs |
| [IAnnotationsDB.h](/src/lib/facade/public/IAnnotationsDB.h) | the annotations database itself: load, store, merge, identity |
| [IImagesPathsDBProvider.h](/src/lib/facade/public/IImagesPathsDBProvider.h) | the read and query half of the database |
| [ImageRecord.h](/src/lib/facade/public/ImageRecord.h) | one annotated image |
| [ImageRecordRect.h](/src/lib/facade/public/ImageRecordRect.h) | one named rectangle drawn over an image |
| [IRecordsFactory.h](/src/lib/facade/public/IRecordsFactory.h) | the extension seam of the deserialization |
| [IDBMerger.h](/src/lib/facade/public/IDBMerger.h) | the extension seam of the merging |
| [LibraryContext.h](/src/lib/facade/public/LibraryContext.h) | the in and out parameters holder |
| [ILib.h](/src/lib/facade/public/ILib.h) | the one-shot `libcall(ctx)` entry point |
| [IRecord.h](/src/lib/facade/public/IRecord.h), [IProvider.h](/src/lib/facade/public/IProvider.h) | the empty tagging bases every record and every provider derives from |

Each interface header also declares a `...Ptr` alias next to the class - `iadd::IAnnotationsDBPtr`, `iadd::ImageRecordPtr`, `iadd::ImageRecordRectPtr`, `iadd::IRecordsFactoryPtr`, `iadd::IDBMergerPtr`, `iadd::LibraryContextPtr`, `iadd::ILibPtr` - and they are all `std::shared_ptr` specializations. Everything the library hands out is shared owned; there is nothing to `delete` and nothing to hand back.

### `LibraryFacade` - the front door

`iadd::LibraryFacade` is a class of static methods only. It is never instantiated, it holds no state and every call to it builds a brand new independent object.

| Method | Returns |
| --- | --- |
| `create_library_context()` | a `LibraryContextPtr` pre-filled with the compile time defaults |
| `create_default_lib()` | the default `ILib` implementation |
| `create_library(ctx)` | the `ILib` implementation appropriate for the given context |
| `create_annotations_db()` | an empty database carrying the compile time identity defaults |
| `create_annotations_db(ctx)` | an empty database configured by the given context |
| `open_annotations_db(fpath)` | a database with the given project file already loaded, or a `nullptr` |
| `create_records_factory()` | the library default `IRecordsFactory` |
| `create_db_merger()` | the library default `IDBMerger` |
| `library_version()` | the version string of the library binary being used |
| `default_db_root_app_name()` | the compile time `ANNOTATIONS_DB_ROOT_APP_NAME` value |
| `default_db_app_version()` | the compile time `ANNOTATIONS_DB_APP_VERSION` value |

The last three are plain reporting calls, handy for an about box or for a log line at the start-up. The identity pair is explained in the [configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md) section.

### The three ways in

#### 1. `open_annotations_db()` - the common case

One line, one file, done:

```cpp
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

auto db = iadd::LibraryFacade::open_annotations_db("project.json");

if (db == nullptr) {
  // the file was missing, unreadable, not a JSON document at all
  // or not a valid annotations project - see the JSON format section
  return false;
}
```

It creates a default database and immediately calls `load_db()` on it, returning a `nullptr` when that load fails. The database it builds carries the compile time application identity and the library own records factory and merger. When that is all you need - and for a reader, an inspector or a converter it usually is - stop here.

#### 2. `create_annotations_db(ctx)` - when the collaborators matter

Reach for the context form as soon as one of the three things it carries has to be different: the application identity written into the file, the records factory the deserialization builds the records with, or the merger `add_images_db()` performs its job with.

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->app_name = "MyTool";
ctx->app_version = "2.0";

auto db = iadd::LibraryFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("project.json")) {
  return false;
}
```

A freshly created context is already usable: its `app_name` and `app_version` are seeded with the compile time defaults, and its `records_factory` and `merger` are empty, which means "use the library own ones". A `nullptr` context is accepted too and is treated as an empty default one.

Mind the one asymmetry here: `create_annotations_db()` creates an **empty** database. It never looks at `ctx->db_path` - that field belongs to the `libcall()` path described next - so the `load_db()` call is yours to make.

#### 3. `ILib::libcall()` - the one-shot path

`ILib` is the single method interface the library implementation variants are behind. Fill a context, hand it over, and get the ready database back through that very same context:

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->db_path = "project.json";

auto lib = iadd::LibraryFacade::create_library(ctx);

if (lib == nullptr || !lib->libcall(ctx)) {
  return false;
}

auto db = ctx->db;  // an iadd::IAnnotationsDBPtr
```

`libcall()` builds the database out of the context exactly the way `create_annotations_db(ctx)` does, and then, **only when `ctx->db_path` is not empty**, loads that file into it. An empty `db_path` is not an error: you simply get a configured empty database in `ctx->db`.

The `LibraryContext` fields split into the in and the out halves accordingly:

| Field | Direction | Meaning |
| --- | --- | --- |
| `db_path` | in | the project file `libcall()` is to open. Empty leaves the database empty |
| `app_name` | in | the JSON root element name. Defaults to the compile time value |
| `app_version` | in | the version string stored under that root element |
| `records_factory` | in | an optional custom `IRecordsFactory`. Empty means the library default |
| `merger` | in | an optional custom `IDBMerger`. Empty means the library default |
| `db` | out | the database produced by the last `libcall()` |

Use this path when your own architecture already passes a context-like object around, or when a single call that either fully succeeds or fully fails is what fits best. When you want a finer grained control, go through the facade factory methods instead.

### The `IAnnotationsDB` surface

`IAnnotationsDB` extends the read only `IImagesPathsDBProvider`. The split is deliberate: hand the code that only *reads* the records an `iadd::IImagesPathsDBProviderPtr` and it becomes trivially testable, and trivially feedable from an alternative in-memory source of your own.

The read and query half, from `IImagesPathsDBProvider`:

| Method | Behaviour |
| --- | --- |
| `ImageRecordsSet& get_images_db()` | a **modifiable reference** to the held records |
| `std::string get_db_path()` | the path of the project file last loaded or stored, or an empty string |
| `AnnotationsList get_available_annotations()` | the sorted `std::set<std::string>` of every distinct rectangle name in the whole database |
| `bool changed()` | true when the in-memory records differ from the stored ones |
| `bool delete_image_record(const std::string& irFullPath)` | erases every record whose `get_full_path()` equals the argument |

`delete_image_record()` reports the attempt, not the hit count: it returns `true` even when nothing matched. Compare the set size before and after when you need to know.

The loading and storing half, added by `IAnnotationsDB`:

| Method | Behaviour |
| --- | --- |
| `bool load_db(const std::string& fpath)` | parses the file and **replaces** the in-memory records with what it holds. The path becomes the default store destination |
| `bool store_db()` | writes the records back to that last used path |
| `bool store_db(const std::string& fpath)` | writes the records to the given path, which becomes the new default destination |
| `void add_images_db(const ImageRecordsSet& andb)` | merges another records set in through the `IDBMerger` |
| `void set_app_info(const std::string& name, const std::string& version)` | overrides the identity written into and expected inside the files |
| `std::string get_app_name() const` | the JSON root element name currently in use |
| `std::string get_app_version() const` | the version string currently in use |

Note that `load_db()` **replaces** rather than appends. Loading a second project file over the first one throws the first one away; `add_images_db()` is the call that combines two sets.

### `get_images_db()` hands out a mutable reference

This is the point worth being explicit about. `get_images_db()` returns `ImageRecordsSet&` - a live reference into the database, not a copy:

```cpp
for (const auto& ir : db->get_images_db()) {
  for (const auto& rect : ir->rects) {
    rect->name = "cat";          // edits the database in place
  }
}

db->get_images_db().pop_back();  // so does this
```

Records are `std::shared_ptr`s, so editing a record through a copy of the pointer edits the very same record. That is the whole editing API: there are no setters, you mutate the records and then you store them. The consequences to keep in mind:

- the reference and every `ImageRecordPtr` you took out of it are invalidated by a following `load_db()`, which swaps the whole set;
- `add_images_db()` may reallocate the vector, so do not hold on to iterators across it;
- the library is not thread safe by itself. One database instance driven from several threads needs your own synchronization.

Need a detached copy to play with instead? `iadd::ImageRecord::duplicate(set)` deep copies a whole set, and `duplicate_shared()` does the same for a single record or rectangle.

### The record value types

`ImageRecord` is one annotated image. It is a plain data holder with a handful of conveniences:

| Member | Meaning |
| --- | --- |
| `std::string abs_dir_path` | the root directory the record came from - or the web page URL |
| `std::string path` | the image path relative to `abs_dir_path` |
| `std::filesystem::path tmppath` | the local cache path of an image preloaded from the network. **Never** written into the project file |
| `ImageRecordRectSet rects` | the annotation rectangles |
| `ImageRecordRectPtr current_rect` | the rectangle the presentation layer currently has selected. Not serialized either |
| `int iwidth`, `int iheight` | the intrinsic image size |
| `double imageScale` | the per-image preview scale. `0` means "no user-selected scale yet" |

Its methods: `get_full_path()` returns `tmppath` when the image was preloaded from the network and `abs_dir_path` joined with `path` otherwise - that is the value `delete_image_record()` matches against and the value the records are sorted by; `erase_current_rect()` drops the selected rectangle; `scaleStepIn()` / `scaleStepOut()` walk `imageScale` by `defaultScaleStep` within the `minScale` .. `maxScale` bounds; `duplicate_shared()`, `duplicate()` and the two static `equal()` overloads do the deep copying and the value wise comparison.

`ImageRecordRect` is a named rectangle - `name`, `x`, `y`, `width`, `height` - plus the same `duplicate_shared()` and static `equal()` helpers. The coordinates are in the image own pixel space, so they stay valid no matter what preview scale the presentation layer applies.

Both types are constructible directly (`iadd::ImageRecord::create(...)` and the `ImageRecordRect` constructors are public), but prefer the factory - see below - so that a custom one stays in charge.

### The `changed()` / `store_db()` contract

The database keeps a deep copy of the records as they were at the last successful load or store. `changed()` compares the live set against that snapshot **value wise**, record by record and rectangle by rectangle:

- a record counts as equal when its `abs_dir_path`, `path`, `imageScale`, `iwidth`, `iheight` and its whole rectangle sequence match;
- rectangles must match in name, geometry **and order**;
- `tmppath` and `current_rect` are not part of the comparison. They are presentation state and are never serialized.

Which means `changed()` is honest in both directions: undo an edit by hand and it goes back to reporting `false`. Reorder two rectangles and it reports `true`, because the file would come out different.

A successful `store_db()` refreshes the snapshot, so `changed()` returns `false` right after it. A failed one does not touch the snapshot, and the pending modifications stay pending. The idiom is therefore:

```cpp
if (db->changed() && !db->store_db()) {
  // the write failed - the data is still in memory, do not exit silently
  return false;
}
```

One trap: the argument less `store_db()` writes to the path of the last load or store, and a database created by `create_annotations_db()` has never had one. Calling it there fails. Give a freshly built database an explicit destination once:

```cpp
auto db = iadd::LibraryFacade::create_annotations_db();

db->add_images_db(my_records);

if (!db->store_db("/tmp/new-project.json")) {
  return false;
}

// that path is the default now, the plain store_db() works from here on
```

### Extension seam 1: `IRecordsFactory`

Every record the deserialization produces is built through an `IRecordsFactory`. Substitute your own and the JSON converters start handing you your own enriched subclasses - which is how you attach whatever your application needs (a loaded pixbuf, a database id, a dirty flag) to a record without the library knowing a thing about it.

The interface is three methods:

```cpp
virtual ImageRecordPtr create_image_record(const std::string& nirpath = "",
                                           const std::string& nabsPath = "") = 0;

virtual ImageRecordRectPtr create_image_rect_record(
    const ImageRecordPtr& ir = nullptr) = 0;

virtual ImageRecordRectPtr create_image_rect_record(const std::string& nname,
                                                    const int& nx, const int& ny,
                                                    const int& nwidth,
                                                    const int& nheight) = 0;
```

The first one takes the path relative to the root and the root itself. The second creates an **empty** rectangle with an auto generated name: with a `nullptr` record it is the `default_name_record` constant (`"rectangle_default"`), otherwise it is `default_numbered_name_record_prefix` followed by the current rectangle count of the given record, so a fresh rectangle for an image that has two already comes out as `rectangle_default_2`. The third one builds a fully defined rectangle, and it is the one the JSON deserialization calls.

A custom factory does not have to reimplement anything - delegate to the library default one and touch only what you care about:

```cpp
#include <cctype>
#include <memory>
#include <string>

#include <ImagesAnnotatorDataDrivers-0.11/IRecordsFactory.h>
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

/// Builds exactly the records the library default factory builds, but forces
/// every annotation class name to the lower case.
class LowerCaseRecordsFactory : public iadd::IRecordsFactory
{
 public:
  LowerCaseRecordsFactory()
      : defaults{iadd::LibraryFacade::create_records_factory()}
  {
  }

  iadd::ImageRecordPtr create_image_record(const std::string& relPath,
                                           const std::string& absPath) override
  {
    return defaults->create_image_record(relPath, absPath);
  }

  iadd::ImageRecordRectPtr create_image_rect_record(
      const iadd::ImageRecordPtr& ir) override
  {
    return defaults->create_image_rect_record(ir);
  }

  iadd::ImageRecordRectPtr create_image_rect_record(const std::string& name,
                                                    const int& x, const int& y,
                                                    const int& width,
                                                    const int& height) override
  {
    return defaults->create_image_rect_record(lowered(name), x, y, width,
                                              height);
  }

 private:
  static std::string lowered(std::string s)
  {
    for (auto& c : s) {
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    return s;
  }

  iadd::IRecordsFactoryPtr defaults;
};
```

### Extension seam 2: `IDBMerger`

`IDBMerger` is one method - `merge(ImageRecordsSet& dst, const ImageRecordsSet& append)` - and it defines what `add_images_db()` means. The library default implementation appends only the records whose resolved path is not present in the destination yet and then sorts the destination by the records full paths. "Resolved" is the interesting word there: a record pointing at a web URL is resolved through the libcurl `CURLU` URL API before the comparison, so the same image referenced by a bare relative path against a page URL and by its absolute URL is recognised as one record.

Replace it when your rules differ - say you are collecting several partial annotation passes over the same images and want them all kept:

```cpp
#include <ImagesAnnotatorDataDrivers-0.11/IDBMerger.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

/// Appends everything, duplicates included, and preserves the given order.
class AppendAllMerger : public iadd::IDBMerger
{
 public:
  void merge(iadd::ImageRecordsSet& dst,
             const iadd::ImageRecordsSet& append) override
  {
    dst.reserve(dst.size() + append.size());

    for (const auto& ir : append) {
      dst.emplace_back(ir->duplicate_shared());
    }
  }
};
```

### Plugging them in

Both seams are injected through the context, and both are optional - leave a field empty and the library default is used:

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->records_factory = std::make_shared<LowerCaseRecordsFactory>();
ctx->merger = std::make_shared<AppendAllMerger>();

auto db = iadd::LibraryFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("project.json")) {
  return false;
}

// the records were built by your factory during the load above
db->add_images_db(another_set);  // and merged by your merger here
```

The same context goes to `ILib::libcall()` if you prefer the one-shot path. Note once more that `open_annotations_db()` takes no context and therefore always uses the library defaults - it is the shorthand, not the configurable route.

### Where to go next

- The on-disk shape of what all of this reads and writes: [the annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md).
- The identity the produced files carry: [configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md).
- Building your own program against the installed package: [using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md).
- The proof that the public headers are enough on their own: the `CTEST_AnnotationsDirDB` component test at [src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB) links the real shared library and reaches it through those headers exclusively. It is a fine source of further usage examples.
