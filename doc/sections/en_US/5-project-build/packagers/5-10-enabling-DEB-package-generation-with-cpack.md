## Enabling DEB package generation with cpack

In order to be able to generate the `deb` package file configure project to support the `cpack` command feature with a `ENABLE_DEB` variable enabled:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_DEB=ON
```

Next, build all available targets required for the `deb` package

```
# inside the project root directory

cmake --build build --target all
```

Finally, execute the `cpack` command inside the project build directory:

```
# inside the project build directory

cpack
```

The package file should be generated inside the project build root directory. For example, if project name wasn't changed and it's version is `0.14.0` so the package name may look like `ImagesAnnotatorDataDrivers-0.14.0-amd64-ubuntu-24.04.deb`.

After the version the package name carries the destination CPU architecture, the one of the package `Architecture` field as well. The `TEMPLATE_PROJECT_DEB_PACKAGE_ARCHITECTURE` cache variable holds it (default: the `dpkg --print-architecture` output, e.g. `amd64` or `arm64`), so override it when cross compiling the project or packaging it on a host without the `dpkg`.

Unless the project is cross compiled, the architecture is followed by the name and the version of the destination operating system, that is of the build host itself: the `ID` and the `VERSION_ID` fields of its `/etc/os-release` file, e.g. `ubuntu-24.04` or `debian-13` (a rolling release like the Debian sid names its `ID` alone). The `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable, shared by every packager of the project, holds it. A cross compiled build leaves it empty, so its package names the architecture alone; set it to name the destination system yourself (e.g. `-DTEMPLATE_PROJECT_PACKAGE_SYSTEM=debian-13`), or to an empty string to name none.

The package carries what the project installs, that is the shared library `libImagesAnnotatorDataDrivers-0.14.so.0.14.0` with its symlinks, the public headers under `include/ImagesAnnotatorDataDrivers-0.14/` and the CMake package files - everything a downstream `find_package(ImagesAnnotatorDataDrivers-0.14 REQUIRED)` needs. There is no executable in it, this project does not build one.

In order to examine details of the `deb` package configuration visit the [cmake/enablers/packagers/template-project-deb-enabler.cmake](/cmake/enablers/packagers/template-project-deb-enabler.cmake) file.
