## The library's installable include header files

The [src/lib/facade/public](/src/lib/facade/public) directory contains the installable public include header files. They define the whole API surface of the library - nothing else of the project is reachable from the outside - and every declaration inside them belongs to the `ImagesAnnotatorDataDrivers011` namespace.

| Header | What it declares |
|---|---|
| [LibraryFacade.h](/src/lib/facade/public/LibraryFacade.h) | The static factory front door: `create_library_context`, `create_default_lib`, `create_library`, `create_annotations_db` (twice), `open_annotations_db`, `create_records_factory`, `create_db_merger`, plus the `library_version`, `default_db_root_app_name` and `default_db_app_version` reporters |
| [LibraryContext.h](/src/lib/facade/public/LibraryContext.h) | The data carried into and out of a library call: the `db_path`, `app_name`, `app_version`, `records_factory` and `merger` inputs and the resulting `db` output |
| [ILib.h](/src/lib/facade/public/ILib.h) | The `libcall(ctx)` one shot entry point interface |
| [IAnnotationsDB.h](/src/lib/facade/public/IAnnotationsDB.h) | The central abstraction: the read side of its base plus `load_db`, `store_db` (twice), `add_images_db`, `set_app_info`, `get_app_name` and `get_app_version` |
| [IImagesPathsDBProvider.h](/src/lib/facade/public/IImagesPathsDBProvider.h) | The read and query side: `get_images_db`, `get_db_path`, `get_available_annotations`, `changed` and `delete_image_record` |
| [IRecordsFactory.h](/src/lib/facade/public/IRecordsFactory.h) | The records creation policy: `create_image_record` and the two `create_image_rect_record` overloads |
| [IDBMerger.h](/src/lib/facade/public/IDBMerger.h) | The merge policy: the single `merge(dst, append)` method |
| [ImageRecord.h](/src/lib/facade/public/ImageRecord.h) | One annotated image: `abs_dir_path`, `path`, `tmppath`, `rects`, `current_rect`, `iwidth`, `iheight` and `imageScale` |
| [ImageRecordRect.h](/src/lib/facade/public/ImageRecordRect.h) | One named rectangle: `name`, `x`, `y`, `width` and `height` |
| [IRecord.h](/src/lib/facade/public/IRecord.h), [IProvider.h](/src/lib/facade/public/IProvider.h) | The empty tagging base classes of the two hierarchies above |

The contents of the [src/lib/facade/public](/src/lib/facade/public) directory are installed under the `include/<library name>` sub-directory of the chosen install prefix, which with the current project name and major version means `include/ImagesAnnotatorDataDrivers-0.11/` - see [Changing the project and the library name](/doc/sections/en_US/4-project-structure/4-2-changing-the-project-and-executable-name.md) and [Customizing the installable library name segments](/doc/sections/en_US/5-project-build/compression/5-23-customizing-library-name-segments.md).

### Both include forms work

The library target exports both the plain include root and its versioned sub-directory, so an installed consumer may write either of these:

```
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>   // recommended
#include <LibraryFacade.h>
```

Prefer the prefixed form. Header names such as `ImageRecord.h` or `IRecord.h` are generic enough to collide in a busy include path, and the prefix also makes it obvious which library version a translation unit is talking to. The headers include each other by their plain names, which keeps working in both cases because the versioned sub-directory is exported as an include directory of its own.

### The namespace carries the version

The public namespace is `ImagesAnnotatorDataDrivers011` - the major and the minor version of the library are stamped into its name on purpose, so that two versions of the library may be present in a single translation unit without their types clashing. Downstream code is encouraged to alias it:

```
namespace iadd = ImagesAnnotatorDataDrivers011;
```

### The CMake package

Alongside the successful installation of the library binary and the header files there is installed an appropriate CMake package to ensure the library visibility for the external projects requiring it: `find_package(ImagesAnnotatorDataDrivers-0.11 REQUIRED)` brings in the `ImagesAnnotatorDataDrivers-0.11::ImagesAnnotatorDataDrivers-0.11` imported target, which carries both of the include directories mentioned above. Examine the [src/lib/cmake/lib-cmake-module-gen.cmake](/src/lib/cmake/lib-cmake-module-gen.cmake) and [src/lib/cmake/TemplateLibraryConfig.cmake.in](/src/lib/cmake/TemplateLibraryConfig.cmake.in) CMake files for the details on the library project CMake package generation, and the [Using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) section for a complete consumer example.

Note that neither the nlohmann json nor the libcurl dependency appears in any of these headers: both are linked `PRIVATE`, so a downstream project inherits neither of them and needs neither of them on its include path.
