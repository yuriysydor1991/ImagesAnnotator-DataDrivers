## Project tests

The library is covered by the tests kept next to the code they exercise: every component directory has its own `tests/unit` and, where it makes sense, `tests/component` subdirectory. Both kinds are off by default and are enabled by a separate CMake option each, so building the library itself never requires GoogleTest.

```
# from the project root - both kinds at once

cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

### Unit tests versus component tests

The two kinds differ in what exactly they are testing, and that difference is visible in their `CMakeLists.txt` files:

- **The unit tests** (`-DENABLE_UNIT_TESTS=ON`, target names prefixed with `UTEST_`) do not link the produced shared library at all. Every such executable names the very translation units of the component under test and compiles them straight from the tree, while all of its collaborators are replaced by the gmock stand-ins from [src/tests/mocks](/src/tests/mocks) (the logger, the `project-global-decls.h`, the `LibraryContext` and the `LibMain`) or by the per-component mocks kept in the component own `tests/mocks` directory. That gives the tests access to the internal headers and lets them isolate a single class.
- **The component tests** (`-DENABLE_COMPONENT_TESTS=ON`, target names prefixed with `CTEST_`) do the opposite: `CTEST_AnnotationsDirDB` links the real `ImagesAnnotatorDataDrivers-0.11` shared library and reaches it through the installable public headers only - no mock and no internal header is on its include path. It is therefore a test of the library exactly as a downstream consumer sees it, driving real project JSON files taken from its `ctest_data` directory.

### The test targets

| Target | Kind | Location |
|---|---|---|
| `UTEST_LibraryFacade` | unit | [src/lib/facade/tests/unit/LibraryFacade](/src/lib/facade/tests/unit/LibraryFacade) |
| `UTEST_LibFactory` | unit | [src/lib/libmain/tests/unit/LibFactory](/src/lib/libmain/tests/unit/LibFactory) |
| `UTEST_LibMain` | unit | [src/lib/libmain/tests/unit/LibMain](/src/lib/libmain/tests/unit/LibMain) |
| `UTEST_ImageRecord` | unit | [src/records/tests/unit/ImageRecord](/src/records/tests/unit/ImageRecord) |
| `UTEST_ImageRecordRect` | unit | [src/records/tests/unit/ImageRecordRect](/src/records/tests/unit/ImageRecordRect) |
| `UTEST_RecordsFactory` | unit | [src/records/tests/unit/RecordsFactory](/src/records/tests/unit/RecordsFactory) |
| `UTEST_AnnotationsJSONSerializator` | unit | [src/dbs/AnnotationsDBs/tests/unit/AnnotationsJSONSerializator](/src/dbs/AnnotationsDBs/tests/unit/AnnotationsJSONSerializator) |
| `UTEST_LibraryImpl` | unit | [src/dbs/AnnotationsDBs/tests/unit/LibraryImpl](/src/dbs/AnnotationsDBs/tests/unit/LibraryImpl) |
| `UTEST_DefaultDBMerger` | unit | [src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger](/src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger) |
| `UTEST_ImageRecords2JSONConverter` | unit | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecords2JSONConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecords2JSONConverter) |
| `UTEST_JSON2ImageRecordsConverter` | unit | [src/dbs/AnnotationsDBs/tests/unit/converters/JSON2ImageRecordsConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/JSON2ImageRecordsConverter) |
| `UTEST_ImageRecordJSON2RecordConverter` | unit | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordJSON2RecordConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordJSON2RecordConverter) |
| `UTEST_ImageRecordRectJSON2RecordConverter` | unit | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordRectJSON2RecordConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordRectJSON2RecordConverter) |
| `CTEST_AnnotationsDirDB` | component | [src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB) |
| `CTEST_log` | component | [src/log/tests/component/log](/src/log/tests/component/log) |
| `CTEST_DefaultLogger` | component | [src/log/default-logger/tests/component/DefaultLogger](/src/log/default-logger/tests/component/DefaultLogger) |

Every configured executable registers its cases with CTest through the `gtest_add_tests()` command, so `ctest -R UTEST_DefaultDBMerger` and friends work as expected. See the [Run tests by the ctest](/doc/sections/en_US/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md) subsection for the ways of running them and the [Project build](/doc/sections/en_US/5-project-build/5-project-build.md) subsection for the ways of enabling them.
