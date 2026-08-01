## Enabling the libcurl

The [libcurl](https://curl.se/libcurl/) is a **mandatory** dependency of this project, so the `ENABLE_LIBCURL` CMake option defaults to `ON` and a plain configure already brings the library in:

```
# Inside the source root directory

cmake -S . -B build
```

Should you ever need to pass it explicitly (a CI script that spells out every option, a build cache reused from an older configure that has the variable stored as `OFF`), set the `ON` value to the `ENABLE_LIBCURL` CMake variable:

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LIBCURL=ON
```

Switching it `OFF` does not produce a leaner library, it produces one that does not compile. The option is kept only because the whole third party enabling machinery of the project is built around such options.

### What the library actually uses it for

Not for downloading anything. An annotations database source folder is identified by the `db.path` element of the project file, and that path is allowed to be a web URL just as well as a local absolute directory path. Telling those two apart, splitting a URL into its parts, joining a record's relative path onto it and normalizing the result back into a comparable string is exactly what libcurl's **CURLU URL API** (`curl_url()`, `curl_url_set()`, `curl_url_get()` and friends) exists for - and writing a correct URL parser by hand is a famously bad idea.

That API is wrapped by `curli::CURLController` in [src/lib/libmain/CURL](/src/lib/libmain/CURL) and used by `helpers::ImageRecordUrlAndPathHelper` in [src/lib/libmain/helpers](/src/lib/libmain/helpers). The practical consequence is in the merging: `DefaultDBMerger` has to decide whether an incoming image record is the same image as one already in the database, and for web hosted records that decision is made on the normalized URL. Without the helper, two spellings of the same remote image would end up as two separate records.

### Where the library comes from

The system installed libcurl is probed first with a `find_package(CURL QUIET)`. If the system has it - and on a typical GNU/Linux development host it does - that copy is used and nothing is downloaded. Only when the probe fails does the project fall back to `FetchContent`:

- `TEMPLATE_APP_CURL_GIT` - the git repository, `https://github.com/curl/curl.git` by default,
- `TEMPLATE_APP_CURL_GIT_TAG` - the git tag, `master` by default. Building curl itself from source is a slow and fairly demanding affair, so installing the system package (`libcurl4-openssl-dev` on the Debian family, `libcurl-devel` on the RPM one) is by far the preferable route. If you do end up on the fallback path, consider pinning that variable to a release tag of your choice.

### The enabler / linker module split

Just like the JSON dependency, this one is handled by **two** CMake modules instead of one:

- [cmake/enablers/template-project-libcurl-enabler.cmake](/cmake/enablers/template-project-libcurl-enabler.cmake) declares the `ENABLE_LIBCURL` option and only makes the dependency *available*. It is included from [cmake/template-project-feature-enabler.cmake](/cmake/template-project-feature-enabler.cmake), which runs **before** `src/` is added to the build - the test executables declared inside `src/` compile the helper sources straight from the tree and have to be able to link libcurl themselves, at a moment when the library target does not exist yet.
- [cmake/enablers/template-project-libcurl-linker.cmake](/cmake/enablers/template-project-libcurl-linker.cmake) does the actual `target_link_libraries()` against the library target. It is included from [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), which runs **after** `src/` has been added.

### The linkage is PRIVATE

```
target_link_libraries(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE CURL::libcurl
)
```

No installable public header under [src/lib/facade/public](/src/lib/facade/public) exposes a libcurl type - the `CURLU` handles never leave the implementation. Downstream projects consuming the installed package therefore inherit neither the include path nor the link dependency and do not need libcurl on their own include path in order to use this library.
