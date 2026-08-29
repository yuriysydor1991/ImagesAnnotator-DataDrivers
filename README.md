**The ImagesAnnotator project annotations database library**

# What is it

The `ImagesAnnotatorDataDrivers` is a C++17 shared library which reads and writes the annotations project files of the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application - the JSON documents that keep the annotated images, their sizes and the named rectangles drawn over them.

The code used to live inside the application itself. It was extracted into a library of its own so that the ImagesAnnotator application and **any other tool** may read and write one and the same project file format without duplicating the parser, the serializer and the merging rules.

Everything a consuming project touches is hidden behind the abstract interfaces of the [src/lib/facade/public](/src/lib/facade/public) headers, so no implementation class, and none of the library third party dependencies, leak into the downstream code.

See more at the [kytok.org.ua](http://www.kytok.org.ua/)

💵 Donate at [http://kytok.org.ua/page/pozertvy](http://kytok.org.ua/page/pozertvy)

# Features

- **Load and store the annotations project JSON file** - `load_db()` parses a project file into the memory, `store_db()` writes it back either to the very same path or to a new one.
- **An in-memory image records set** - every annotated image is an `ImageRecord` holding its source directory (or web page URL), the relative path, the intrinsic width and height, the preview scale and the `ImageRecordRect` named rectangles.
- **Change tracking** - `changed()` tells whether the in-memory records still match the stored ones, so an application may skip the write or ask the user about the unsaved data.
- **Duplicate aware merging** - `add_images_db()` appends another records set through an `IDBMerger`, skipping the records already present and keeping the result sorted. Web hosted records are resolved through the libcurl URL API, so the same image referenced by two differently spelled URLs is recognised as one.
- **Annotation names collection** - `get_available_annotations()` returns the sorted set of every distinct annotation (class) name used across the whole database.
- **Record deletion** - `delete_image_record()` erases a record by its full path.
- **A pluggable records factory and merger** - put your own `IRecordsFactory` and/or `IDBMerger` descendant into the `IADataDriversContext` and the deserialization will build your enriched record subclasses and merge them by your own rules.
- **A configurable JSON root element identity** - the project file is rooted at an element named after the *application*, not after the library, so the name and the version stay configurable both at the build time and at the run time.

# Usage example

The library is consumed through a CMake package. The following program was compiled, linked and run against the installed library:

```cmake
cmake_minimum_required(VERSION 3.13)
project(MyTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ImagesAnnotatorDataDrivers-0.12 REQUIRED)

add_executable(mytool main.cpp)
target_link_libraries(mytool ImagesAnnotatorDataDrivers-0.12::ImagesAnnotatorDataDrivers-0.12)
```

```cpp
#include <iostream>
#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers012;

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

The `ImagesAnnotatorDataDrivers012` namespace name carries the library major and minor version numbers on purpose: two library versions may coexist inside a single translation unit without any symbol clash. Alias it, as shown above, and the version bump stays a one line change on your side.

Both `#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>` and a plain `#include <IADataDriversFacade.h>` work for an installed consumer, since the library exports the include root along with its versioned subdirectory. The prefixed form is the recommended one: header names like `ImageRecord.h` or `IRecord.h` are generic enough to collide in a busy include path.

More on the API and on the file format itself is in the [annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md), the [annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md) and the [using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) documentation sections.

# Dependencies

Both of the following are **mandatory** for the library to build and both default to `ON`:

| CMake option | Library | Why it is needed |
| --- | --- | --- |
| `ENABLE_NLOHMANN_JSON` | [nlohmann/json](https://github.com/nlohmann/json) | the annotations project file is a JSON document |
| `ENABLE_LIBCURL` | [libcurl](https://curl.se/libcurl/) | its `CURLU` URL API resolves and de-duplicates the image records that point at a web URL instead of a local file |

Each of them is first probed system wide and, when it is not found, is pulled by the CMake `FetchContent` (the nlohmann json at the `v3.11.3` tag). See the [enabling the nlohmann json library](/doc/sections/en_US/5-project-build/5-18-enabling-the-nlohmann-json-library.md) and the [enabling the libcurl](/doc/sections/en_US/5-project-build/5-14-enabling-libcurl.md) sections for the source and tag overriding variables.

Both are linked **privately**: no public header exposes a nlohmann or a curl type, so a downstream project inherits neither of them and needs neither of them on its include path. The logging component is compiled straight into the shared library as well, so no logger implementation has to be supplied by the consumer.

# Build and test

The plain build:

```
cmake -S . -B build
cmake --build build -j$(nproc)
```

The tests are off by default. To build and run them:

```
cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

The `ENABLE_UNIT_TESTS` targets are compiled straight from the sources against the gmock stand-ins of [src/tests/mocks](/src/tests/mocks), while the `ENABLE_COMPONENT_TESTS` `CTEST_AnnotationsDirDB` links the real shared library and drives it through the public headers only - exactly the way a downstream project does.

Installing is a usual `sudo cmake --install build`, described in detail in the [installing](/doc/sections/en_US/7-installing/7-installing.md) section.

# Where the code came from

The sources were lifted out of the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application, namely its `src/annotator-business/dbs/AnnotationsDBs` component. The application is to drop its own copy and to consume this library instead.

The dataset exporters of that application (the plain text, the YOLO v4 and the PyTorch-Vision ones) are **not** a part of this library: they are to become a separate project of their own.

Examine the `doc` directory for possible translations of a current md document:
- `uk_UA` at [doc/README.uk_UA.md](doc/README.uk_UA.md)

# Documentation contents

**Document is under the refinement**

1. [Cloning the project](/doc/sections/en_US/1-cloning-the-project/1-cloning-the-project.md)
1. [Forking and replacing the origin](/doc/sections/en_US/2-forking-and-replacing-the-origin/2-forking-and-replacing-the-origin.md)
1. [Requirements](/doc/sections/en_US/3-requirements/3-requirements.md)
    1. [Required tools for the GNU/Linux based OS](/doc/sections/en_US/3-requirements/3-1-required-tools-for-the-GNU-Linux-based-OS.md)
    1. [Required tools for the MS Windows based OS](/doc/sections/en_US/3-requirements/3-2-required-tools-for-the-MS-Windows-based-OS.md)
    1. [Optional for the tests](/doc/sections/en_US/3-requirements/3-3-optional-for-the-tests.md)
    1. [Optional for the documentation](/doc/sections/en_US/3-requirements/3-4-optional-for-the-documentation.md)
    1. [Optional for the code formatting](/doc/sections/en_US/3-requirements/3-5-optional-for-the-code-formatting.md)
    1. [Optional for the code analyzer (cppcheck)](/doc/sections/en_US/3-requirements/3-6-optional-for-the-code-analyzer-cppcheck.md)
    1. [Optional for the code analyzer with clang-tidy](/doc/sections/en_US/3-requirements/3-7-optional-for-the-code-analyzer-with-clang-tidy.md)
    1. [Optional for the memory check with Valgrind](/doc/sections/en_US/3-requirements/3-8-optional-for-the-memory-checkwith-Valgrind.md)
    1. [Optional for the flatpak packager](/doc/sections/en_US/3-requirements/3-9-optional-for-the-flatpak-packager.md)
    1. [Optional for the docker container runs](/doc/sections/en_US/3-requirements/3-10-optional-for-docker-container-runs.md)
    1. [Optional for the snap packager](/doc/sections/en_US/3-requirements/3-11-optional-for-snap-packager.md)
1. [Project structure](/doc/sections/en_US/4-project-structure/4-project-structure.md)
    1. [Project diagrams](/doc/sections/en_US/4-project-structure/4-0-project-diagrams.md)
    1. [Implement code straight away!](/doc/sections/en_US/4-project-structure/4-1-implement-code-straight-away.md)
    1. [The library's installable include header files](/doc/sections/en_US/4-project-structure/4-8-the-librarys-installable-include-header-files.md)
    1. [The annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md)
    1. [The annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md)
    1. [Changing the project and executable name](/doc/sections/en_US/4-project-structure/4-2-changing-the-project-and-executable-name.md)
    1. [Version tracking and other project parameters](/doc/sections/en_US/4-project-structure/4-3-version-tracking-and-other-project-parameters.md)
    1. [Minimal possible versions](/doc/sections/en_US/4-project-structure/4-6-minimal-possible-versions.md)
    1. [Project tests](/doc/sections/en_US/4-project-structure/4-4-project-tests.md)
        1. [Google Test](/doc/sections/en_US/4-project-structure/4-4-1-google-test.md)
    1. [Extensions](/doc/sections/en_US/4-project-structure/4-5-extensions.md)
1. [Project build](/doc/sections/en_US/5-project-build/5-project-build.md)
    1. [IDE build](/doc/sections/en_US/5-project-build/5-1-IDE-build.md)
    1. [Command line build](/doc/sections/en_US/5-project-build/5-2-command-line-build.md)
    1. [Quick build scripts](/doc/sections/en_US/5-project-build/5-37-quick-build-scripts.md)
    1. [Configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md)
    1. Enabling testing
        1. [Enabling unit testing](/doc/sections/en_US/5-project-build/testing/5-3-1-enabling-unit-testing.md)
        1. [Disabling system GTest probe](/doc/sections/en_US/5-project-build/testing/5-3-2-disabling-system-GTest-probe.md)
    1. [Documentation build](/doc/sections/en_US/5-project-build/documentation/5-4-documentation-build.md)
    1. [Configuring the documentation install support](/doc/sections/en_US/5-project-build/documentation/5-5-configuring-the-documentation-install-support.md)
    1. [Customizing the installable library name segments](/doc/sections/en_US/5-project-build/compression/5-23-customizing-library-name-segments.md)
    1. Code quality & sanitizers
        1. [Enabling and performing code formatting target](/doc/sections/en_US/5-project-build/code-quality/5-6-enabling-and-performing-code-formatting-target.md)
        1. [Enabling the static code analyzer target with cppcheck](/doc/sections/en_US/5-project-build/code-quality/5-7-enabling-the-static-code-analyzer-target-with-cppcheck.md)
        1. [Enabling the static code analyzer with clang-tidy](/doc/sections/en_US/5-project-build/code-quality/5-8-enabling-static-code-analyzer-with-clang-tidy.md)
        1. [Enabling the dynamic memory check target with valgrind](/doc/sections/en_US/5-project-build/code-quality/5-9-enabling-the-dynamic-memory-check-target-with-valgrind.md)
        1. [Enabling sanitizers](/doc/sections/en_US/5-project-build/code-quality/5-13-enabling-sanitizers.md)
    1. Containers & CI
        1. [Enabling the docker container build and run](/doc/sections/en_US/5-project-build/containers-ci/5-12-enabling-the-docker-container-build-and-run.md)
        1. [Enabling Jenkins pipeline inside Docker container](/doc/sections/en_US/5-project-build/containers-ci/5-17-enabling-Jenkins-pipeline-inside-Docker-container.md)
    1. Profilers
        1. [Enabling gprof profiler analysis](/doc/sections/en_US/5-project-build/profilers/5-15-enabling-gprof-profiler-analysis.md)
        1. [Enabling valgrind's callgrind profiler analysis](/doc/sections/en_US/5-project-build/profilers/5-16-enabling-valgrinds-callgrind-profiler-analysis.md)
    1. Packagers
        1. [Enabling DEB package generation with cpack](/doc/sections/en_US/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md)
        1. [Enabling the flatpak package generation support](/doc/sections/en_US/5-project-build/packagers/5-11-enabling-the-flatpak-package-generation-support.md)
        1. [Enabling the snap package manager](/doc/sections/en_US/5-project-build/packagers/5-19-enabling-the-snap-package-manager.md)
        1. [Enabling FreeBSD pkg package generation with cpack](/doc/sections/en_US/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md)
        1. [Enabling WIX MSI package generation with cpack](/doc/sections/en_US/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md)
        1. [Enabling RPM package generation with cpack](/doc/sections/en_US/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md)
    1. Libraries
        1. [Enabling the libcurl](/doc/sections/en_US/5-project-build/5-14-enabling-libcurl.md)
        1. [Enabling the nlohmann json library](/doc/sections/en_US/5-project-build/5-18-enabling-the-nlohmann-json-library.md)
    1. Security / Cryptography
        1. [Enabling the OpenSSL library](/doc/sections/en_US/5-project-build/security/5-35-enabling-the-openssl-library.md)
    1. Compression
        1. [Enabling the zlib library](/doc/sections/en_US/5-project-build/compression/5-23-enabling-the-zlib-library.md)
        1. [Enabling the liblzma library](/doc/sections/en_US/5-project-build/compression/5-34-enabling-the-liblzma-library.md)
    1. Images
        1. [Enabling the libpng library](/doc/sections/en_US/5-project-build/image-libraries/5-24-enabling-the-libpng-library.md)
        1. [Enabling the libjpeg library](/doc/sections/en_US/5-project-build/image-libraries/5-25-enabling-the-libjpeg-library.md)
        1. [Enabling the libwebp library](/doc/sections/en_US/5-project-build/image-libraries/5-26-enabling-the-libwebp-library.md)
        1. [Enabling the lunasvg library (SVG)](/doc/sections/en_US/5-project-build/image-libraries/5-27-enabling-the-lunasvg-library.md)
        1. [Enabling the giflib library (GIF)](/doc/sections/en_US/5-project-build/image-libraries/5-28-enabling-the-giflib-library.md)
        1. [Enabling the libtiff library (TIFF)](/doc/sections/en_US/5-project-build/image-libraries/5-29-enabling-the-libtiff-library.md)
        1. [Enabling the OpenEXR library (EXR / HDR)](/doc/sections/en_US/5-project-build/image-libraries/5-30-enabling-the-openexr-library.md)
        1. [Enabling the OpenJPEG library (JPEG 2000)](/doc/sections/en_US/5-project-build/image-libraries/5-31-enabling-the-openjpeg-library.md)
        1. [Enabling the libavif library (AVIF)](/doc/sections/en_US/5-project-build/image-libraries/5-32-enabling-the-libavif-library.md)
        1. [Enabling the libheif library (HEIF/HEIC)](/doc/sections/en_US/5-project-build/image-libraries/5-33-enabling-the-libheif-library.md)
1. [Run available executables](/doc/sections/en_US/6-run-the-executable/6-run-avaialble-executables.md)
    1. [Run the executable](/doc/sections/en_US/6-run-the-executable/6-run-the-executable.md)
        1. [IDE run](/doc/sections/en_US/6-run-the-executable/6-1-IDE-run.md)
        1. Command line run
            1. [Searching for the generated executable](/doc/sections/en_US/6-run-the-executable/6-2-1-searching-for-the-generated-executable.md)
            1. [Starting the generated executable](/doc/sections/en_US/6-run-the-executable/6-2-2-starting-the-generated-executable.md)
    1. Tests run
        1. [Run tests by the ctest](/doc/sections/en_US/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md)
        1. [Manual tests run](/doc/sections/en_US/6-run-the-executable/6-3-2-manual-tests-run.md)
1. [Installing](/doc/sections/en_US/7-installing/7-installing.md)
1. [Using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md)
