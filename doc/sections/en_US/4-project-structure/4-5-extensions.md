## Extensions

Beyond the library itself the project ships a set of optional CMake integrations, all of them living in the [cmake](/cmake) directory and all of them driven by a plain configure time option, so that none of them is in the way of a plain build. The pieces are:

- **The dependency enablers** in [cmake/enablers](/cmake/enablers). Each of them probes the system for a library first and falls back to a `FetchContent` download of a pinned git tag. Two of them are mandatory for this project and default to `ON` - the [nlohmann json](/doc/sections/en_US/5-project-build/5-18-enabling-the-nlohmann-json-library.md) one, since the annotations project file is a JSON document, and the [libcurl](/doc/sections/en_US/5-project-build/5-14-enabling-libcurl.md) one, used through its URL API to resolve the image records pointing at a web page. The rest (the compression, the image format and the OpenSSL ones) are inherited leftovers kept for the projects growing out of this one and are `OFF` by default.
- **The code quality targets**: the `clang-format` formatting target, the `cppcheck` and the `clang-tidy` static analyzers and the sanitizers - see the code quality subsections of the [Project build](/doc/sections/en_US/5-project-build/5-project-build.md) section.
- **The packagers** built on top of CPack: DEB, RPM, FreeBSD pkg and WIX MSI, plus the flatpak and snap helpers kept in [cmake/enablers/packagers](/cmake/enablers/packagers).
- **The containers and the CI helpers** in [cmake/enablers/dockerers](/cmake/enablers/dockerers), together with the [misc/Jenkinsfile](/misc/Jenkinsfile) pipeline which builds the project, runs the tests and verifies that every markdown link of this documentation resolves.
- **The documentation build** through Doxygen, optionally installable along with the library.
- **The tests**, which are an integration of the same kind: `ENABLE_UNIT_TESTS` and `ENABLE_COMPONENT_TESTS`, both `OFF` by default. See [Project tests](/doc/sections/en_US/4-project-structure/4-4-project-tests.md).

The full list with the configure recipe of every single one of them is in the [Project build](/doc/sections/en_US/5-project-build/5-project-build.md) section.

The integrations are not a framework in any traditional means: an item which is redundant for your fork may be altered or simply dropped, and new ones are going to be introduced in the future.
