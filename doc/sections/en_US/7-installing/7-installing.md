
# Installing

Execute available install commands from the project's build directory.

## Default installation

In order to install the generated library (as shown previous) into your system default folders, execute next command in the command line (GNU/Linux based):

```
# installs the generated library under the /usr/local/lib/ for example
# and installs the header include files under the
# /usr/local/include/ImagesAnnotatorDataDrivers-0.14/

sudo cmake --install .
```

Usually it's the `/usr/local/lib` directory for the library (on the Unix-like OS), which is not necessarily on the default loader search path of your distribution - run `sudo ldconfig` afterwards, or install into `/usr` as shown below, should the produced binary not be found at the run time.

## Installation by the quick build scripts

The scripts of the [Quick build scripts](/doc/sections/en_US/5-project-build/5-37-quick-build-scripts.md) section perform the install step themselves when the `--install` parameter is given, so the whole configure, build and install cycle takes the single command:

```
# inside the project root directory

scripts/build/release.sh --install
```

The install step alone, against the already built `build/release` directory, is performed by its stage script:

```
# inside the project root directory

scripts/build/release-install.sh --install
```

Both of them install into the `/usr` prefix by the `sudo cmake --install` call, so the `sudo` password is asked for. Without the `--install` parameter the install step is skipped entirely and no password is ever asked.

## Custom installation path

To install the library into a system globally available directory add the `--prefix` parameter to the command above as next:

```
# replace the /usr path with your own if needed
# run from the project's build directory

sudo cmake --install . --prefix "/usr"
```

Alternatively, any path may be specified - a local prefix under your home directory works just as well, as long as the consuming project is pointed at it with the `CMAKE_PREFIX_PATH` variable.

## What gets installed

With the `/usr` prefix of the example above the installed tree is:

```
/usr/lib/libImagesAnnotatorDataDrivers-0.14.so.0.14.0     the library binary
/usr/lib/libImagesAnnotatorDataDrivers-0.14.so.0          the SOVERSION symlink
/usr/lib/libImagesAnnotatorDataDrivers-0.14.so            the development symlink
/usr/lib/cmake/ImagesAnnotatorDataDrivers-0.14/
    ImagesAnnotatorDataDrivers-0.14Config.cmake           the find_package() entry point
    ImagesAnnotatorDataDrivers-0.14Config-noconfig.cmake
    ImagesAnnotatorDataDrivers-0.14Targets.cmake          the exported imported target
    ImagesAnnotatorDataDrivers-0.14Targets-noconfig.cmake
/usr/include/ImagesAnnotatorDataDrivers-0.14/*.h          the public headers
```

The `*.h` files of the last line are the eleven public headers of the [src/lib/facade/public](/src/lib/facade/public) directory: `IADataDriversFacade.h`, `IADataDriversContext.h`, `IADataDriversLib.h`, `IAnnotationsDB.h`, `IImagesPathsDBProvider.h`, `IRecordsFactory.h`, `IDBMerger.h`, `ImageRecord.h`, `ImageRecordRect.h`, `IRecord.h` and `IProvider.h`.

The `ImagesAnnotatorDataDrivers-0.14` name of the binary, of the include sub-directory and of the CMake package is the `PROJECT_LIBRARY_NAME` value, so all of them follow the [customized name segments](/doc/sections/en_US/5-project-build/compression/5-23-customizing-library-name-segments.md) should you configure any.

The library binary itself is installed as the `Runtime` component and the `lib...so` development symlink as the `Development` one, which is what a distribution packager needs to split the runtime and the development packages apart.

Once installed, point your own project at the package with `find_package(ImagesAnnotatorDataDrivers-0.14 REQUIRED)` - see the [Using the library in your project](/doc/sections/en_US/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) section for a complete, verified example.

## Documentation install

If project was configured to support the documentation install by the command which looks like next:

```
# inside the project build directory

cmake ../ -DENABLE_DOC_DOXYGEN=ON -DDOXYGEN_DO_INSTALL=ON
```

The installation command (for example, described in the [Default installation](#default-installation) section) will install the generated HTML documentation files into appropriate directories.
