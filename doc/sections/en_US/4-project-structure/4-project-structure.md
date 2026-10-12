# Project structure

The repository holds a single CMake project named `ImagesAnnotatorDataDrivers` which produces exactly one binary artefact - the `ImagesAnnotatorDataDrivers-0.12` shared library - along with its installable public headers and its CMake package files. No executable is generated here.

A bird's eye view of the tree:

```
.
├── CMakeLists.txt     the root project() declaration: the name, the version, the C++17 standard
├── cmake/             the whole CMake infrastructure: the dependency enablers, the compile
│                      options, the packagers and the code analyzer targets
├── doc/               this documentation, the diagrams and the Doxygen setup
├── misc/              the CI helpers: the Jenkinsfile, the Dockerfiles, the packagers data
└── src/               the sources
```

And the `src/` directory itself, which is where all the interesting parts live:

```
src/
├── project-global-decls.h.in            the configured compile time constants
├── lib/
│   ├── cmake/                           the installable CMake package generation
│   ├── facade/
│   │   ├── public/                      the installable public headers - the whole API surface
│   │   ├── IADataDriversFacade.cpp      the static front door implementation
│   │   └── IADataDriversContext.cpp
│   └── libmain/
│       ├── LibMain.{h,cpp}              the default IADataDriversLib implementation  namespace lib0impl
│       └── LibFactory.{h,cpp}           the implementation factory                   namespace lib0impl
├── records/                             ImageRecord, ImageRecordRect, RecordsFactory
│                                                                namespace iannotator::records
├── helpers/                             IHelper, SortHelper, UrlHelper,
│                                        ImageRecordUrlAndPathHelper        namespace helpers
├── zlib/                                ZlibController, GzipFileStreams of the cpp-app-template
│                                                                namespace iadd0zlibi
├── dbs/AnnotationsDBs/                  AnnotationsDirDB, LibraryImpl, AnnotationsJSONSerializator,
│   │                                    AnnotationsDBTypes
│   │                                                      namespace iannotator::dbs::annotations
│   ├── converters/                      the JSON to records and back converters
│   └── mergers/                         DefaultDBMerger
├── log/                                 the logging component
└── tests/mocks/                         the shared gmock stand-ins
```

A few things are worth knowing about that layout:

- Everything under [src/lib/facade/public](/src/lib/facade/public) is installed and is the only thing a consumer ever includes. Every declaration in there belongs to the `ImagesAnnotatorDataDrivers012` namespace, whose name carries the library major and minor version on purpose - see [The library's installable include header files](/doc/sections/en_US/4-project-structure/4-8-the-librarys-installable-include-header-files.md).
- Everything else under [src](/src) is the implementation: [src/lib/libmain](/src/lib/libmain) holds the entry point pair, and the components it drives sit beside it as [src/records](/src/records), [src/helpers](/src/helpers), [src/zlib](/src/zlib) and [src/dbs](/src/dbs). It lives in its own namespaces (`lib0impl`, `iannotator::records`, `iannotator::dbs::annotations`, `helpers`, `iadd0zlibi`), none of its headers is installed and nothing outside of the library binary is able to reach it.
- The [src/log](/src/log) logging component is compiled straight into the produced shared library (its `default-logger` is linked in as an object library), so a consumer does not have to supply any logger implementation of its own to link against the library.
- The test sources sit next to the code they cover, inside the per-component `tests/unit` and `tests/component` subdirectories, and the mocks shared between them live in [src/tests/mocks](/src/tests/mocks). See the [Project tests](/doc/sections/en_US/4-project-structure/4-4-project-tests.md) subsection.

The subsections below are walking through the parts of the project in more detail:

1. [Project diagrams](/doc/sections/en_US/4-project-structure/4-0-project-diagrams.md)
1. [Where the implementation lives](/doc/sections/en_US/4-project-structure/4-1-implement-code-straight-away.md)
1. [The library's installable include header files](/doc/sections/en_US/4-project-structure/4-8-the-librarys-installable-include-header-files.md)
1. [The annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md)
1. [The annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md)
1. [Changing the project and the library name](/doc/sections/en_US/4-project-structure/4-2-changing-the-project-and-executable-name.md)
1. [Version tracking and other project parameters](/doc/sections/en_US/4-project-structure/4-3-version-tracking-and-other-project-parameters.md)
1. [Minimal possible versions](/doc/sections/en_US/4-project-structure/4-6-minimal-possible-versions.md)
1. [Project tests](/doc/sections/en_US/4-project-structure/4-4-project-tests.md)
    1. [Google Test](/doc/sections/en_US/4-project-structure/4-4-1-google-test.md)
1. [Extensions](/doc/sections/en_US/4-project-structure/4-5-extensions.md)
