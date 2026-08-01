### Manual tests run

Currently tests are separated in different files named with the `UTEST_*` pattern for the unit tests and the `CTEST_*` pattern for the component ones.

You may search for all compiled available tests by a next command:

```
# from the project root or a build directory

find -type f -executable -name 'UTEST_*' -o -type f -executable -name 'CTEST_*'
```

After that, choose particular test of interest and execute it manually if needed. For example, for the `LibraryFacade` class pass it's UT relative file system path from a project's root directory into the command line and hit enter (GNU/Linux based):

```
# from the project root

./build/src/lib/facade/tests/unit/LibraryFacade/UTEST_LibraryFacade
```

The component test of the annotations directory database is started exactly the same way. Unlike the unit tests it links the real produced shared library and drives it through the installable public headers only, so what it exercises is the very same code path a downstream consumer of the library would take:

```
# from the project root

./build/src/lib/libmain/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/CTEST_AnnotationsDirDB
```

Every one of these binaries is an ordinary GoogleTest executable, so `--gtest_list_tests` shows what is inside it and `--gtest_filter=<pattern>` runs a subset:

```
# from the project root

./build/src/lib/libmain/records/tests/unit/ImageRecord/UTEST_ImageRecord --gtest_list_tests
./build/src/lib/libmain/records/tests/unit/ImageRecord/UTEST_ImageRecord --gtest_filter='*full_path*'
```
