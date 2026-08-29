## Configuring the annotations database identity

Every annotations project file is rooted at an element named after the **application** that owns the format, and carries a version string under it:

```json
{
  "ImagesAnnotator": {
    "version": "0.12.0"
  },
  "annotations": [ ... ]
}
```

That pair - the root element name and the version string - is what this section calls the *database identity*. Two CMake cache variables set its compile time defaults, and both are declared in [cmake/template-project-misc-variables-declare.cmake](/cmake/template-project-misc-variables-declare.cmake):

| CMake variable | Default | What it becomes |
| --- | --- | --- |
| `ANNOTATIONS_DB_ROOT_APP_NAME` | `ImagesAnnotator` | the JSON root element name of the produced files, and the name a reader looks the root element up by |
| `ANNOTATIONS_DB_APP_VERSION` | the project version, currently `0.12.0` | the version string written under that root element and compared against the one found in a file being read |

Both are forwarded into the generated `project-global-decls.h` through [src/project-global-decls.h.in](/src/project-global-decls.h.in), and both are readable from a consuming program through `IADataDriversFacade::default_db_root_app_name()` and `IADataDriversFacade::default_db_app_version()`.

### Why the default is not the library name

This is the part worth understanding before changing anything.

The library was extracted out of the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application, and the project files that application has been writing all along are rooted at an element literally named `"ImagesAnnotator"`. The root element is looked up **by name**: a reader that does not find the name it expects refuses the document.

So if the library stamped its own name in there - `"ImagesAnnotatorDataDrivers"` - it would produce files that the very application it exists to serve could no longer open, and it would refuse to open the files that application already wrote. Which is why the default is the *application* name and not the library one, and why it is a configurable value rather than a hard coded literal.

The rule of thumb: **leave both variables alone** unless you are deliberately driving a differently named file format of your own. Changing the name is not a cosmetic tweak, it is a format fork.

### Changing them at the configure time

Both are ordinary cache variables, so they go on the `cmake` command line:

```
# Inside the source root directory

cmake -S . -B build \
  -DANNOTATIONS_DB_ROOT_APP_NAME="MyTool" \
  -DANNOTATIONS_DB_APP_VERSION="2.0"
```

Every database instance built by that library binary then defaults to the `"MyTool"` root element and the `"2.0"` version stamp. Verify what a built library actually carries without opening a single file:

```cpp
#include <iostream>
#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers012;

std::cout << iadd::IADataDriversFacade::library_version() << " writes '"
          << iadd::IADataDriversFacade::default_db_root_app_name() << "' rooted files, version "
          << iadd::IADataDriversFacade::default_db_app_version() << "\n";
```

Bear in mind that a compile time change is baked into the shared library and therefore affects **every** consumer of that installed binary. A distribution package of the library should ship the defaults. If what you actually need is one differently stamped file out of your own program, use the run time route below instead - it needs no rebuild at all and does not step on anybody else.

`ANNOTATIONS_DB_APP_VERSION` alone is far less dangerous than the name: a version mismatch between the configured value and the one found in a file only produces a warning in the log, and the file loads anyway. See the [annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md) section for that behaviour.

### Changing them at the run time

The compile time values are only the defaults. Two run time overrides are available and neither of them requires rebuilding the library.

**Through the library context**, before the database is created:

```cpp
auto ctx = iadd::IADataDriversFacade::create_library_context();

ctx->app_name = "MyTool";
ctx->app_version = "2.0";

auto db = iadd::IADataDriversFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("my-tool-project.json")) {
  return false;
}
```

A freshly created context already holds the compile time defaults in those two fields, so overwrite only what you want different. The same context works with the one-shot `IADataDriversLib::libcall()` path.

**Through the database itself**, at any moment:

```cpp
auto db = iadd::IADataDriversFacade::create_annotations_db();

db->set_app_info("MyTool", "2.0");

std::cout << db->get_app_name() << " " << db->get_app_version() << "\n";
```

Both routes end up in the same place - `set_app_info()` is exactly what the context form calls under the hood.

### When the identity is consulted

Timing matters here, because the identity is read at the operation, not stored with the data:

- **On loading**, the configured name is the key the root element is searched for, and the configured version is what the file version is compared against. So `set_app_info()` has to be called *before* `load_db()` for it to have any effect on that load.
- **On storing**, the configured name and version are written out as they are at that very moment. Nothing of what a previously loaded file carried is remembered.

The second point has a practical consequence: loading a `0.10.0` stamped file and storing it back with the defaults produces a `0.12.0` stamped one. When preserving the original stamp matters, read it back and put it in place before storing:

```cpp
auto db = iadd::IADataDriversFacade::open_annotations_db("project.json");

if (db == nullptr) {
  return false;
}

// keep whatever identity this instance is configured with, but do not
// silently bump the version of a file written by an older application
db->set_app_info(db->get_app_name(), db->get_app_version());
```

And the mirror image of it: to convert a file from one application identity to another, load it under the old name and store it under the new one.

```cpp
auto ctx = iadd::IADataDriversFacade::create_library_context();

ctx->app_name = "ImagesAnnotator";  // the identity the input file carries

auto db = iadd::IADataDriversFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("input.json")) {
  return false;
}

db->set_app_info("MyTool", "2.0");  // the identity the output is to carry

return db->store_db("output.json");
```

### See also

- [The annotations project JSON format](/doc/sections/en_US/4-project-structure/4-10-the-annotations-project-json-format.md) - where the identity element sits and what an invalid document is.
- [The annotations database API](/doc/sections/en_US/4-project-structure/4-9-the-annotations-database-api.md) - `IADataDriversContext`, `set_app_info()` and the rest of the surface used above.
- [Version tracking and other project parameters](/doc/sections/en_US/4-project-structure/4-3-version-tracking-and-other-project-parameters.md) - the other values the configure step bakes into `project-global-decls.h`.
