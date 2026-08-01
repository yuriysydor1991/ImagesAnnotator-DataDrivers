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

The package file should be generated inside the project build root directory. For example, if project name wasn't changed and it's version is `0.11.0` so the package name may look like `ImagesAnnotatorDataDrivers-0.11.0-Linux.deb`.

The package carries what the project installs, that is the shared library `libImagesAnnotatorDataDrivers-0.11.so.0.11.0` with its symlinks, the public headers under `include/ImagesAnnotatorDataDrivers-0.11/` and the CMake package files - everything a downstream `find_package(ImagesAnnotatorDataDrivers-0.11 REQUIRED)` needs. There is no executable in it, this project does not build one.

In order to examine details of the `deb` package configuration visit the [cmake/enablers/packagers/template-project-deb-enabler.cmake](/cmake/enablers/packagers/template-project-deb-enabler.cmake) file.
