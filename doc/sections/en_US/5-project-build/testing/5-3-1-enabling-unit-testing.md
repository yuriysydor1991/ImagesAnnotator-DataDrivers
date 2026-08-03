### Enabling unit testing

To enable project unit test availability (for building and running) reconfigure it with enabled `ENABLE_UNIT_TESTS` variable as follows (GNU/Linux based):

```
# from the project root

cmake -B build -S . -DENABLE_UNIT_TESTS=ON && cmake --build build
```

The unit tests do **not** link the produced shared library. They compile the very same translation units straight from the tree and substitute the gmock stand-ins living under [src/tests/mocks](/src/tests/mocks) for the neighbouring components, which is what lets them poke at a single class in isolation.

The following unit test executables are produced:

| Target | What it covers |
|---|---|
| `UTEST_LibraryFacade` | the static front door of the public API |
| `UTEST_LibFactory` | the factory behind the facade |
| `UTEST_LibMain` | the one-shot `libcall(ctx)` entry point |
| `UTEST_ImageRecord` | a single annotated image record |
| `UTEST_ImageRecordRect` | a single named rectangle of a record |
| `UTEST_RecordsFactory` | the records factory implementation |
| `UTEST_AnnotationsJSONSerializator` | the project file JSON serializator |
| `UTEST_DefaultDBMerger` | the default databases merger |
| `UTEST_ImageRecords2JSONConverter` | image records -> JSON |
| `UTEST_JSON2ImageRecordsConverter` | JSON -> image records |
| `UTEST_ImageRecordJSON2RecordConverter` | a single JSON element -> image record |
| `UTEST_ImageRecordRectJSON2RecordConverter` | a single JSON element -> rectangle |

There is a second, independent switch for the component tests, `ENABLE_COMPONENT_TESTS`. Those go the other way around: `CTEST_AnnotationsDirDB` links the real produced shared library and drives it through the installable public headers only, with no mocks and no access to the internals, exactly the way a downstream consumer would. `CTEST_DefaultLogger` covers the logger compiled into the library.

Both switches default to `OFF` and are perfectly happy together - enable both to get the full suite:

```
# from the project root

cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

Either of the two options is enough to trigger the GoogleTest framework probe and the `enable_testing()` call, see the [Disabling system GTest probe](/doc/sections/en_US/5-project-build/testing/5-3-2-disabling-system-GTest-probe.md) subsection for the details of where the framework comes from. On how to run what was built refer to the [Run tests by the ctest](/doc/sections/en_US/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md) and [Manual tests run](/doc/sections/en_US/6-run-the-executable/6-3-2-manual-tests-run.md) subsections.
