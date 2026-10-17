# Using the library in your project

This section is the downstream integration guide: what to install, what to write in your own `CMakeLists.txt`, how to include the headers and what a complete working program looks like. The CMake project and the program shown below were compiled, linked and run against the installed library.

You do not need the library sources to use it. Configure, build and install them once - or install a package built out of them - and your own project only ever sees the installed CMake package.

## 1. Install the library

The build and the install are the usual ones, described in detail in the [project build](/doc/sections/en_US/5-project-build/5-project-build.md) and the [installing](/doc/sections/en_US/7-installing/7-installing.md) sections:

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
cd ImagesAnnotator-DataDrivers

cmake -S . -B build
cmake --build build -j$(nproc)

sudo cmake --install build --prefix "/usr"
```

That leaves the following behind:

| Installed | Path (for the `/usr` prefix above) |
| --- | --- |
| the shared library | `/usr/lib/libImagesAnnotatorDataDrivers-0.14.so.0.14.0`, with the `.so.0` soname link and the `.so` development link |
| the public headers | `/usr/include/ImagesAnnotatorDataDrivers-0.14/` - all eleven of them |
| the CMake package | `/usr/lib/cmake/ImagesAnnotatorDataDrivers-0.14/` |

Drop the `--prefix` and everything lands under `/usr/local` instead, which is the default and is perfectly fine as long as your toolchain looks there.

Two things not to worry about: the library needs no logger implementation from you - the logging component is compiled straight into it - and none of its three dependencies leaks out. nlohmann/json, libcurl and zlib are linked **privately**, no public header mentions a type of theirs, so your project puts none of them on its include path and calls `find_package` for none of them. libcurl and the system zlib do remain ordinary shared library dependencies of the installed binary, so they have to stay present on the host, the same way libstdc++ does.

## 2. The consumer `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.13)
project(MyTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ImagesAnnotatorDataDrivers-0.14 REQUIRED)

add_executable(mytool main.cpp)
target_link_libraries(mytool ImagesAnnotatorDataDrivers-0.14::ImagesAnnotatorDataDrivers-0.14)
```

Four things are going on there, and all four matter:

- **C++17 is required.** The public headers use `std::filesystem` and inline static data members. An older standard will not compile them.
- **The package is named after the library, major version included** - `ImagesAnnotatorDataDrivers-0.14`. The version is a part of the name on purpose: a future `ImagesAnnotatorDataDrivers-1` installs beside it instead of over it.
- **The imported target is namespaced with that same name**, hence the slightly redundant looking `ImagesAnnotatorDataDrivers-0.14::ImagesAnnotatorDataDrivers-0.14`. Linking the target is all you have to do: it carries the include directories along with it, so no `target_include_directories` of your own is needed.
- **Ask for the package without a version request.** The installed package directory holds the config and the exported targets files only, so a `find_package(ImagesAnnotatorDataDrivers-0.14 0.14 REQUIRED)` with a version in it does not resolve. The major version baked into the package name is what keeps the incompatible releases apart, and `IADataDriversFacade::library_version()` reports the exact one at the run time.

If the library went into a prefix your CMake does not search by itself, point it there when configuring your own project:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/.local"
```

And a note for the exotic cases: the installed name is not carved in stone. The `LIB_INCLUDE_MINOR_IN_NAME`, `LIB_INCLUDE_MICRO_IN_NAME` and `LIB_NAME_SUFFIX` options of the library build change the binary name, the header sub-directory, the CMake package directory and the imported target namespace all at once - see [customizing the installable library name segments](/doc/sections/en_US/5-project-build/5-23-customizing-library-name-segments.md). Build the library with them and the `find_package` name in your project has to follow, e.g. `ImagesAnnotatorDataDrivers-0.14`.

## 3. Including the headers

Both of these work for an installed consumer, because the package exports the include root together with its versioned sub-directory:

```cpp
#include <ImagesAnnotatorDataDrivers-0.14/IADataDriversFacade.h>   // recommended
#include <IADataDriversFacade.h>                                   // also works
```

Prefer the prefixed form. Header names like `ImageRecord.h`, `IRecord.h` or `IADataDriversLib.h` are generic enough to collide with something else in a busy include path, and the prefix makes it obvious at a glance where a header came from.

`IADataDriversFacade.h` pulls in everything a normal consumer needs - the context, the database interfaces and the record types come along with it. Include the individual headers directly when you implement one of the extension seams, e.g. `<ImagesAnnotatorDataDrivers-0.14/IDBMerger.h>`.

Then there is the namespace. Every public type lives in `ImagesAnnotatorDataDrivers014`, where the `014` is the library major and minor version glued together, so that two library versions may coexist inside a single translation unit. Alias it once:

```cpp
namespace iadd = ImagesAnnotatorDataDrivers014;
```

A version bump then costs you exactly one line.

## 4. The complete example program

`main.cpp` - lists every image record of a project file with its rectangle count, and rewrites the file only when something actually changed:

```cpp
#include <iostream>
#include <ImagesAnnotatorDataDrivers-0.14/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers014;

int main(int argc, char** argv)
{
  if (argc < 2) { return 1; }

  auto db = iadd::IADataDriversFacade::open_annotations_db(argv[1]);

  if (db == nullptr) { return 1; }

  for (const auto& ir : db->get_images_db()) {
    std::cout << ir->get_full_path() << " rects=" << ir->rects.size() << "\n";
  }

  return db->changed() ? (db->store_db() ? 0 : 1) : 0;
}
```

Build and run it:

```
cmake -S . -B build
cmake --build build -j$(nproc)

./build/mytool /path/to/annotations-project.json
```

Small as it is, the program already shows the whole life cycle:

- `open_annotations_db()` creates a database and loads the file in one call, returning a `nullptr` when the file is missing, unreadable or not a valid annotations project;
- `get_images_db()` hands out a **modifiable reference** to the records, so the loop is also the place where you would edit `ir->rects` - the records are shared pointers and there is no separate "commit" step;
- `changed()` compares the live records against the snapshot taken at the load, so the `store_db()` only happens when the write would actually produce something different.

Everything else - the context based construction, the `IADataDriversLib::libcall()` one-shot path, the record fields, the `IRecordsFactory` and `IDBMerger` extension seams - is covered in [the annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md) section, and the file the program reads is specified in [the annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md) one.

## 5. Writing a fresh project file

The other half of the job - building a database from scratch rather than reading one - is just as short. Build the records through the factory, hand them over, name a destination:

```cpp
#include <ImagesAnnotatorDataDrivers-0.14/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers014;

bool write_project(const std::string& outPath)
{
  auto factory = iadd::IADataDriversFacade::create_records_factory();
  auto db = iadd::IADataDriversFacade::create_annotations_db();

  if (factory == nullptr || db == nullptr) { return false; }

  auto ir = factory->create_image_record("cats/001.png", "/data/images");

  ir->iwidth = 1920;
  ir->iheight = 1080;
  ir->rects.emplace_back(
      factory->create_image_rect_record("cat", 10, 20, 100, 200));

  db->add_images_db({ir});

  return db->store_db(outPath);
}
```

Mind the destination: a database that has never been loaded from or stored to a file has no default path, so the first store has to be the `store_db(fpath)` overload. From there on the argument less `store_db()` writes to that same place.

## 6. How ImagesAnnotator itself consumes it

The code in this library used to be the `src/annotator-business/dbs/AnnotationsDBs` component of the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application. The application is to drop that in-tree copy and to consume the installed library instead, through exactly the `find_package` and namespaced target shown above.

The migration is mostly a matter of names rather than of logic, because the extracted interfaces kept theirs: `IAnnotationsDB`, `IImagesPathsDBProvider`, `ImageRecord`, `ImageRecordRect`, `IRecordsFactory` and `IDBMerger` are the very same abstractions the application was already coded against. What changes is where they come from - the installed `ImagesAnnotatorDataDrivers-0.14/` headers instead of a local relative include - and that they now sit in the versioned `ImagesAnnotatorDataDrivers014` namespace, which an alias absorbs. The concrete `AnnotationsDirDB` construction is replaced by an `IADataDriversFacade` call, and the JSON root element identity keeps its `"ImagesAnnotator"` default precisely so that every project file written by every previous release of the application keeps opening - see [configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md).

The dataset exporters of that application - the plain text, the YOLO v4 and the PyTorch-Vision ones - are **not** a part of this library and are not planned to be: they are to become a separate project of their own.

## 7. If it does not link

A few of the usual suspects:

- `find_package` cannot find the package - the prefix is not searched. Pass `-DCMAKE_PREFIX_PATH=<prefix>`, and check that `<prefix>/lib/cmake/ImagesAnnotatorDataDrivers-0.14/ImagesAnnotatorDataDrivers-0.14Config.cmake` is really there.
- The target name is rejected - remember it is doubled: `ImagesAnnotatorDataDrivers-0.14::ImagesAnnotatorDataDrivers-0.14`. And if the library was built with the name segment options, the whole name differs.
- The headers are not found although the package was - you are most probably linking the plain library name instead of the namespaced imported target, and so getting none of its usage requirements.
- The program builds but does not start, complaining about `libImagesAnnotatorDataDrivers-0.14.so.0` - the install prefix `lib` directory is not in the dynamic loader search path. Run `sudo ldconfig` after the install, or set `LD_LIBRARY_PATH` for a non standard prefix.
