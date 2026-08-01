## Enabling the nlohmann JSON library

The annotations project database file **is** a JSON document, so the [nlohmann](https://github.com/nlohmann/json.git) JSON library is a **mandatory** dependency of this project - without it there is nothing left to parse and serialize with. Accordingly the `ENABLE_NLOHMANN_JSON` CMake option defaults to `ON` and there is normally nothing at all to do: a plain configure already brings the library in.

```
# Inside the source root directory

cmake -S . -B build
```

Should you ever need to pass it explicitly (a CI script that spells out every option, a build cache reused from an older configure that has the variable stored as `OFF`), set the `ON` value to the `ENABLE_NLOHMANN_JSON` CMake variable:

```
# Inside the source root directory

cmake -S . -B build -DENABLE_NLOHMANN_JSON=ON
```

Switching it `OFF` does not give you a smaller library, it gives you a library that does not compile. The option is kept only because the whole third party enabling machinery of the project is built around such options.

### Where the library comes from

The system installed nlohmann JSON is probed first with a `find_package(nlohmann_json QUIET)`. If the system has it, that copy is used and nothing is downloaded. Only when the probe fails does the project fall back to `FetchContent` and pull the sources from the Internet:

- `TEMPLATE_APP_NLOHMANN_GIT` - the git repository, `https://github.com/nlohmann/json.git` by default,
- `TEMPLATE_APP_NLOHMANN_GIT_TAG` - the git tag, pinned to `v3.11.3` by default.

The pinned tag is a deliberate choice: the fallback path must produce reproducible builds rather than whatever happens to be on the upstream default branch on the day of the build.

### The enabler / linker module split

Unlike the optional dependencies of the project, this one is handled by **two** CMake modules instead of one:

- [cmake/enablers/template-project-nlohmann-json-enabler.cmake](/cmake/enablers/template-project-nlohmann-json-enabler.cmake) declares the `ENABLE_NLOHMANN_JSON` option and only makes the dependency *available*. It is included from [cmake/template-project-feature-enabler.cmake](/cmake/template-project-feature-enabler.cmake), which runs **before** `src/` is added to the build. That ordering matters: the unit test executables declared inside `src/` compile the library components straight from the tree and therefore have to be able to link the JSON library themselves, at a moment when the library target does not exist yet.
- [cmake/enablers/ImagesAnnotator-nlohmann-json-linker.cmake](/cmake/enablers/ImagesAnnotator-nlohmann-json-linker.cmake) does the actual `target_link_libraries()` against the library target. It is included from [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), which runs **after** `src/` has been added, so by then the target is there to link to.

### The linkage is PRIVATE

The JSON library is linked `PRIVATE`:

```
target_link_libraries(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE nlohmann_json::nlohmann_json
)
```

Not a single installable public header under [src/lib/facade/public](/src/lib/facade/public) mentions a nlohmann type - the JSON stays entirely behind the facade. Two things follow from that. Downstream projects consuming the installed package inherit neither the include path nor the link dependency, so they do not need nlohmann JSON at all in order to use this library. And, on the build system side, a `PUBLIC` (or plain signature) link would have put the `FetchContent` provided `nlohmann_json` target into the `INTERFACE_LINK_LIBRARIES` of the exported target and broken the `install(EXPORT ...)` of the project outright.
