## Enabling the snap packager

> **Read this first.** Just like a flatpak, a [SNAP](https://snapcraft.io/docs/) bundles an *application*: the `snapcraft.yaml` it is built from declares an `apps:` section with a `command` to run. This project builds a shared library and no executable at all, so there is nothing here for a snap to start. The snap target is therefore **not wired into the build system** of this project any more - the [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) module is still in the tree, inherited from the application template this project grew out of, but no `include()` pulls it in, so `-DENABLE_SNAP=ON` currently has no effect. For redistributing the library itself use the [DEB](/doc/sections/en_US/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md), [RPM](/doc/sections/en_US/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/en_US/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) or [WIX MSI](/doc/sections/en_US/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) packagers, all of which are wired in and work.

Should the module be included back into [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), the project's `snap` package target designed to generate the project's SNAP binary package distributive is enabled by configuring the project with the `ENABLE_SNAP` CMake variable:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

The target configures the [misc/packagers/snapcraft.yaml.in](/misc/packagers/snapcraft.yaml.in) template into `<build dir>/snap/snapcraft.yaml` and then runs `snapcraft pack` over it, which would produce a bundle named after the snap `name` and `version` fields, e.g. `imagesannotatordatadrivers_0.12.0_amd64.snap`.

Two cache variables tune the metadata: `TEMPLATE_PROJECT_SNAP_GRADE` (default `devel`) and `TEMPLATE_PROJECT_SNAP_CONFINEMENT` (default `strict`).

Note that the snap `name`, the `apps:` entry and its `command` are all composed out of the `PROJECT_BINARY_NAME` variable - the name of the project executable, which this project no longer declares. That variable has to be provided by whoever brings the target back, otherwise the generated `snapcraft.yaml` describes an application that does not exist.

Visit the [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) file to examine the details.
