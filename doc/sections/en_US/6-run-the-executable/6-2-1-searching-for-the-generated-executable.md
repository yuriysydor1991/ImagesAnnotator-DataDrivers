### Searching for the generated library and test binaries

This project builds no executable of its own, so there is no program of the project's name to look for. What the build produces is the shared library and, if the tests were enabled during the configure stage, the test binaries.

To find the produced library in the project build directory execute the `find` command:

```
# from the project build directory

find . -name 'libImagesAnnotatorDataDrivers-0.so*'
```

Which with the default configure options gives three entries, all of them living in the `src/lib` subdirectory of the build tree:

```
./src/lib/libImagesAnnotatorDataDrivers-0.so.0.11.0    the real library binary
./src/lib/libImagesAnnotatorDataDrivers-0.so.0         the SONAME symlink
./src/lib/libImagesAnnotatorDataDrivers-0.so           the linker name symlink
```

The `ImagesAnnotatorDataDrivers-0` part is the library name derived from the project name plus its major version. Replace it with your custom one if you change the name of the project in the root `CMakeLists.txt` file (the `CMAKE_PROJECT_NAME` variable) or if you tune the name segments through the `LIB_INCLUDE_MINOR_IN_NAME` / `LIB_INCLUDE_MICRO_IN_NAME` / `LIB_NAME_SUFFIX` options - see the [Customizing the installable library name segments](/doc/sections/en_US/5-project-build/compression/5-23-customizing-library-name-segments.md) subsection. The configure stage prints the effective name as a `PROJECT_LIBRARY_NAME: ...` status line, so when in doubt just look there.

To find the compiled test binaries, look for the `UTEST_*` (unit tests) and `CTEST_*` (component tests) patterns:

```
# from the project root or a build directory

find -type f -executable -name 'UTEST_*' -o -type f -executable -name 'CTEST_*'
```

They are scattered across the build tree, each one next to the sources it exercises, for example:

```
./src/lib/facade/tests/unit/LibraryFacade/UTEST_LibraryFacade
./src/lib/libmain/records/tests/unit/ImageRecord/UTEST_ImageRecord
./src/lib/libmain/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger/UTEST_DefaultDBMerger
./src/lib/libmain/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/CTEST_AnnotationsDirDB
```

If the `find` command returns nothing at all, the tests were most probably not enabled during the configure stage - see the [Enabling unit testing](/doc/sections/en_US/5-project-build/testing/5-3-1-enabling-unit-testing.md) subsection.
