## Changing the project and the library name

There is no executable in this project - the only binary artefact is the shared library - so the two names worth knowing about are the CMake project name and the library name derived from it.

The project name is declared by the `project()` call of the root [CMakeLists.txt](/CMakeLists.txt) file and is `ImagesAnnotatorDataDrivers`. Changing it there changes the `CMAKE_PROJECT_NAME` value the rest of the build system is built upon.

The installable library name is the `PROJECT_LIBRARY_NAME` CMake cache variable derived in the [cmake/template-project-misc-variables-declare.cmake](/cmake/template-project-misc-variables-declare.cmake) file. By default it takes the `CMAKE_PROJECT_NAME` value and appends the project major version to it, which for the current `0.14.0` version gives the `ImagesAnnotatorDataDrivers-0.14` name. Optional segments (the minor, the micro and an arbitrary trailing suffix) may be appended to it at the configure time - see the [Customizing the installable library name segments](/doc/sections/en_US/5-project-build/5-23-customizing-library-name-segments.md) subsection.

That single name drives every user visible artefact at once:

| Artefact | Derived from the name | With the current defaults |
|---|---|---|
| The library binary | `lib<NAME>.so.<version>` (the `lib` prefix is added automatically) | `libImagesAnnotatorDataDrivers-0.14.so.0.14.0` with the `SOVERSION` `0` |
| The public headers install directory | `include/<NAME>/` | `include/ImagesAnnotatorDataDrivers-0.14/` |
| The CMake package directory | `lib/cmake/<NAME>/` | `lib/cmake/ImagesAnnotatorDataDrivers-0.14/` |
| The `find_package()` name | `find_package(<NAME> REQUIRED)` | `find_package(ImagesAnnotatorDataDrivers-0.14 REQUIRED)` |
| The imported target | `<NAME>::<NAME>` | `ImagesAnnotatorDataDrivers-0.14::ImagesAnnotatorDataDrivers-0.14` |

Keep in mind that renaming the project renames the installed include directory and the CMake package as well, so every downstream consumer of the library has to be adjusted accordingly.

Two things which are deliberately **not** affected by the project name:

- The public C++ namespace `ImagesAnnotatorDataDrivers014` is spelled out in the public headers themselves and carries the major and minor version so that two versions of the library may coexist in one translation unit.
- The JSON root element name written into the annotations project files. It is the separate `ANNOTATIONS_DB_ROOT_APP_NAME` cache variable defaulting to `ImagesAnnotator`, because the produced files must stay readable by the application, not by the library - see [Version tracking and other project parameters](/doc/sections/en_US/4-project-structure/4-3-version-tracking-and-other-project-parameters.md).
