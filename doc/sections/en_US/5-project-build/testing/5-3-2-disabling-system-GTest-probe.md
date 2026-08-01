### Disabling system GTest probe 

To disable the system available GTest framework assets usage set appropriate value to the `GTEST_TRY_SYSTEM_PROBE` CMake variable by executing command like (GNU/Linux based):

```
cmake -B build -S . -DENABLE_UNIT_TESTS=ON -DGTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

During command execution project build system will try to make GTest available through the Internet only for current project with specified version in the [cmake/enablers/template-project-GTest-enabler.cmake](/cmake/enablers/template-project-GTest-enabler.cmake) file. The repository and the tag are held by the `TEMPLATE_APP_GTEST_GIT` and `TEMPLATE_APP_GTEST_GIT_TAG` cache variables, the latter pinned to `v1.16.0` by default.

The very same switch applies to the component tests, so if you enabled those instead of (or along with) the unit tests, the command is:

```
cmake -B build -S . -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON -DGTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

Note that the whole GTest enabler returns early unless at least one of the `ENABLE_UNIT_TESTS` / `ENABLE_COMPONENT_TESTS` options is `ON`, so with the tests off the value of `GTEST_TRY_SYSTEM_PROBE` changes nothing and no framework is looked for or downloaded at all.
