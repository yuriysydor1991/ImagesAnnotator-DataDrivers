## Version tracking and other project parameters

The project's `CMakeLists.txt` files are tracking the current project git repository commit, the library name, the configure date and the project version, and are forwarding them into the project's configure header file located at [src/project-global-decls.h.in](/src/project-global-decls.h.in). The `project-global-decls.h.in` is compiled into the build directory as `project-global-decls.h`, so any implementation source file of the library reaches those values by a simple `#include "project-global-decls.h"`.

Everything declared in there belongs to the `project_decls` namespace and is an ordinary `const std::string`:

| Constant | Filled with | Value with the current defaults |
|---|---|---|
| `project_decls::PROJECT_NAME` | the `PROJECT_LIBRARY_NAME` cache variable | `ImagesAnnotatorDataDrivers-0.14` |
| `project_decls::PROJECT_BUILD_VERSION` | the `PROJECT_VERSION` of the root `project()` call | `0.14.0` |
| `project_decls::PROJECT_BUILD_COMMIT` | the git commit hash the configure was run at | e.g. `966341c...` |
| `project_decls::PROJECT_CONFIGURE_DATE` | the configure timestamp | e.g. `2026-08-01 12:00:00` |
| `project_decls::ANNOTATIONS_DB_ROOT_APP_NAME` | the `ANNOTATIONS_DB_ROOT_APP_NAME` cache variable | `ImagesAnnotator` |
| `project_decls::ANNOTATIONS_DB_APP_VERSION` | the `ANNOTATIONS_DB_APP_VERSION` cache variable | `0.14.0` (the project version) |

The git commit is obtained by the [cmake/enablers/template-project-git-enabler.cmake](/cmake/enablers/template-project-git-enabler.cmake) module and the configure timestamp together with the two annotations database values are declared in [cmake/template-project-misc-variables-declare.cmake](/cmake/template-project-misc-variables-declare.cmake).

The header itself is an implementation detail and is **not** installed. The values a consumer may legitimately be interested in are re-exported through the public facade: `IADataDriversFacade::library_version()` returns `PROJECT_BUILD_VERSION`, `IADataDriversFacade::default_db_root_app_name()` returns `ANNOTATIONS_DB_ROOT_APP_NAME` and `IADataDriversFacade::default_db_app_version()` returns `ANNOTATIONS_DB_APP_VERSION`.

### The two annotations database parameters

The last two entries of the table deserve a word of explanation, because they are not about the library at all.

An annotations project file is a JSON document rooted at an element named after the **application** which produced it. The files written by the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application carry an `ImagesAnnotator` root element, and the reader looks that very element up. Would the library stamp its own name in there, it would produce files the application is no longer able to read, and it would refuse to load the existing ones.

That is why the name is a value and not a constant:

| CMake cache variable | Default | Meaning |
|---|---|---|
| `ANNOTATIONS_DB_ROOT_APP_NAME` | `ImagesAnnotator` | The JSON root element name of the project files: the name written into the produced files and the one expected inside the read ones |
| `ANNOTATIONS_DB_APP_VERSION` | the project version, `0.14.0` | The version string stored under that root element and compared against the one found inside a read file |

Both are compile time defaults only. Every instance may override them at run time, either through the `IADataDriversContext::app_name` and `IADataDriversContext::app_version` fields before creating the database, or afterwards through `IAnnotationsDB::set_app_info(name, version)`. A version mismatch between the file and the configured value is not fatal - it only produces a warning in the log - while a missing root element makes the load fail.

See the [Configuring the annotations database identity](/doc/sections/en_US/5-project-build/5-36-configuring-the-annotations-db-identity.md) subsection for the configure time recipes.
