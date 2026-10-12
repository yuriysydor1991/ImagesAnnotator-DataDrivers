### Starting the test binaries and a program linked against the library

A shared library is not something you start - it is something another program loads. So there are exactly two things to run here: the test binaries this project builds, and your own program linked against the library.

#### Running a test binary directly

If the tests were enabled during the configure stage and compiled successfully, start any of them in the terminal with the path found in the previous subsection:

```
# from the build dir
./src/lib/facade/tests/unit/IADataDriversFacade/UTEST_IADataDriversFacade
```

They are ordinary GoogleTest executables, so all the usual flags apply, `--gtest_list_tests` to see what is inside and `--gtest_filter=...` to run a subset:

```
# from the build dir
./src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger/UTEST_DefaultDBMerger --gtest_list_tests
```

Running them one by one is mostly a debugging convenience though - to run the whole suite in one go use the `ctest` command instead, see the [Run tests by the ctest](/doc/sections/en_US/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md) subsection.

#### Running a program linked against the library

Once the library is installed (see the [Installing](/doc/sections/en_US/7-installing/7-installing.md) section), your own program picks it up through `find_package()` and is started like any other executable:

```
# from your own project's build directory
./mytool /path/to/the/annotations-project.json
```

Two things commonly go wrong on the first attempt, and both are about the dynamic loader rather than about this project:

- `error while loading shared libraries: libImagesAnnotatorDataDrivers-0.13.so.0` means the loader does not look into the directory the library was installed into. Either refresh the loader cache with `sudo ldconfig` after installing into a system prefix, or point the loader at a custom prefix for the run:

```
LD_LIBRARY_PATH=/usr/local/lib ./mytool /path/to/the/annotations-project.json
```

- an install into a non-standard prefix also has to be findable at configure time by your own project, which is what `CMAKE_PREFIX_PATH` is for:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/your/install/prefix
```

For the full worked example of such a program - the `CMakeLists.txt`, the `main.cpp` and the API calls in it - see the [Using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) section.
