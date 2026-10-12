## Command line build

The project uses the CMake build system so a build command should be familiar to the most of the C++ developers around the world!

Of course, project needs to be `git clone`-ed and it's root directory must be opened in the terminal or in your IDE first. After that you may execute next commands:

```
# from the project root

cmake -B build -S . && cmake --build build
```

Which effectively will create a directory named `build` (it's already added to the `.gitignore` list), configure project using the CMake available in the system (see the [Requirements](/doc/sections/en_US/3-requirements/3-requirements.md) section of this `README.md` file) and finally builds all the targets available in the project.

The main build artifact is the shared library itself, produced under the `build/src/lib` directory:

```
build/src/lib/libImagesAnnotatorDataDrivers-0.14.so.0.14.0    the real library binary
build/src/lib/libImagesAnnotatorDataDrivers-0.14.so.0         the SONAME symlink
build/src/lib/libImagesAnnotatorDataDrivers-0.14.so           the linker name symlink
```

The `ImagesAnnotatorDataDrivers-0.14` part is the library name derived from the project name plus its major version, and it may be tuned - see the [Customizing the installable library name segments](/doc/sections/en_US/5-project-build/compression/5-23-customizing-library-name-segments.md) subsection.

Note that the project builds **no executable**: the shared library plus, if the tests were enabled during the configure stage, the `UTEST_*` and `CTEST_*` test binaries is everything there is to build. All three of the library's mandatory third party dependencies (the nlohmann JSON library, the libcurl and the zlib) are enabled by default, so the plain command above is already enough to get a complete build.
