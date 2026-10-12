## The imgajz1 compressed project format

The imgajz1 file (`*.imgajz1`, the **im**a**g**es **a**nnotator **j**son **z**ipped file of the first version) is the second project file format of the library: the regular JSON project file of the [annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md), gzip compressed ([RFC 1952](https://www.rfc-editor.org/rfc/rfc1952)) by the `ZlibController` of the library. Nothing is added to the JSON and nothing is taken out of it: the decompressed imgajz1 file is the very text the JSON project file of the same project keeps, byte for byte.

A project takes about 16 times less disk space as the imgajz1 file than as the JSON one: the project of 20000 images and 60000 rectangles takes 16.3 MB as the JSON file and 0.99 MB as the imgajz1 one, stored in 0.78 seconds instead of 0.64 (the compression of its 16 MB of the JSON text) and opened in 0.72 seconds instead of 0.74 (a release build).

### Which format a file is

The file name extension of the path tells the format, for the load and for the store alike:

| The path | The format |
| --- | --- |
| `*.imgajz1` (of any letter case: `*.IMGAJZ1` too) | the imgajz1 project file, the JSON project file gzip compressed |
| anything else (`*.json` and the paths of no extension included) | the JSON project file |

So a project opened from a `.json` file and stored into a `.imgajz1` one goes on in the imgajz1 format - the argument less `store_db()` writes the format of the path of the last load or store - and the other way round. Nothing of the API changes: `load_db()`, `store_db()`, `open_annotations_db()` and `IADataDriversLib::libcall()` take either path.

```cpp
#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers012;

auto db = iadd::IADataDriversFacade::open_annotations_db("cars.json");

if (db == nullptr || !db->store_db("cars.imgajz1")) {
  return false;
}

// the database goes on with the cars.imgajz1 file from here
return db->store_db();
```

### The stock tools

The imgajz1 file is a plain gzip file of the JSON text, so the stock tools convert the two files into each other and every JSON tool reads the decompressed project:

```
# the JSON project file of the imgajz1 one
gzip -dc cars.imgajz1 > cars.json

# and the imgajz1 project file of the JSON one
gzip -c cars.json > cars.imgajz1

# the class names of the rectangles of the project
gzip -dc cars.imgajz1 | jq -r '.annotations[].annotations[].annotations[].name' | sort -u
```

### Reading and writing

The writer prints the JSON text of the project the JSON project file is written of - pretty printed with the two space indentation - and compresses it into the file at the default zlib level. The text is made before the file is opened, so a string the JSON can't keep (the file name of no valid UTF-8, for example) fails the store with the project file kept as it was.

The reader decompresses the file and reads the JSON text the way the JSON project file is read, with the same validation. It refuses the corrupted and the truncated gzip data, the gzip trailer of the size and the CRC of the data included, while a file of no gzip compression - the JSON project file named `.imgajz1` - is read as it is, the way the `ZlibController` reads such files.

### The implementation

The format is the [Imgajz1LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h), the [LibraryImpl](/src/dbs/AnnotationsDBs/LibraryImpl.h) descendant the `AnnotationsDirDB` performs the load and the store of the `.imgajz1` paths through (its `impl_of()`). The `LibraryImpl` keeps everything of the JSON format - the parse, the validation and the conversion of the project and the printing of its JSON text - and reads and writes that text through its protected virtual `read_text()` and `write_text()`, the two methods the descendant overrides: they decompress and compress the text through the `uncompress_from_file()` and the `compress_to_file()` of the `ZlibController` of [src/zlib](/src/zlib), the zlib controller component of the [cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template) `appZlib` branch in the adaptation the [darknetxx](https://github.com/yuriysydor1991/darknetxx) made of it, which makes the [zlib](/doc/sections/en_US/5-project-build/compression/5-23-enabling-the-zlib-library.md) a mandatory dependency of the library.
