## Enabling the flatpak package generation support

> **Read this first.** A flatpak bundles an *application* - it has an application id, a `command` to start and a sandbox to start it in. This project builds a shared library and no executable at all, so there is nothing here for a flatpak to run. Accordingly the flatpak target is **not wired into the build system** of this project any more: the [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) module is still in the tree, inherited from the application template this project grew out of, but no `include()` pulls it in, so `-DENABLE_FLATPAK=ON` currently has no effect. The section is kept for the sake of anybody who wants to bring the target back for an application built on top of this library. For redistributing the library itself use the [DEB](/doc/sections/en_US/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md), [RPM](/doc/sections/en_US/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/en_US/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) or [WIX MSI](/doc/sections/en_US/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) packagers, all of which are wired in and work.

Should the module be included back into [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), the flatpak generation of the redistribution package is enabled by configuring the project with the `ENABLE_FLATPAK` option (GNU/Linux and alike):

```
# inside the project root directory 

cmake -B build -S . -DENABLE_FLATPAK=ON
```

Which in order will enable the `flatpak` target.

And finally to generate the flatpak package run the `flatpak` target for the build:

```
# inside the project root directory 

cmake --build build --target flatpak
```

In case of the success target build, there will be created the flatpak file under the root build directory named, for example, the `ImagesAnnotatorDataDrivers-0.13.0.flatpak`. In order to examine and/or change the flatpak generation target parameters visit the [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) or [misc/packagers/flatpak.conf.json.in](/misc/packagers/flatpak.conf.json.in) files.

Note that both the bundle file name and the application id are composed out of the `PROJECT_BINARY_NAME` variable - the name of the project executable, which this project no longer declares. The id template is `ua.org.kytok.template.${PROJECT_BINARY_NAME}`, so before the target can produce anything meaningful that variable (and a `command` for the sandbox to start) has to be provided by whoever brings the target back.

Refer to the flatpak documentation on how to install and run applications distributed from a flatpaks files. With the name settled, the install and run commands would look like this:

```
# to install (version may change)
# inside the project build directory
flatpak install --user ImagesAnnotatorDataDrivers-0.13.0.flatpak

# to run the application
flatpak run ua.org.kytok.template.ImagesAnnotatorDataDrivers
```
