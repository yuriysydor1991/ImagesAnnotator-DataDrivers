### Disabling system GTest probe 

To disable the system available GTest framework assets usage set appropriate value to the `IADD_GTEST_TRY_SYSTEM_PROBE` CMake variable by executing command like (GNU/Linux based):

```
cmake -B build -S . -DIADD_ENABLE_UNIT_TESTS=ON -DIADD_GTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

During command execution project build system will try to make GTest available through the Internet only for current project with specified version in the [cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake](/cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake) file. The repository and the tag are held by the `IADD_GTEST_GIT` and `IADD_GTEST_GIT_TAG` cache variables, the latter pinned to `v1.16.0` by default.

The very same switch applies to the component tests, so if you enabled those instead of (or along with) the unit tests, the command is:

```
cmake -B build -S . -DIADD_ENABLE_UNIT_TESTS=ON -DIADD_ENABLE_COMPONENT_TESTS=ON -DIADD_GTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

Note that the whole GTest enabler returns early unless at least one of the `IADD_ENABLE_UNIT_TESTS` / `IADD_ENABLE_COMPONENT_TESTS` options is `ON`, so with the tests off the value of `IADD_GTEST_TRY_SYSTEM_PROBE` changes nothing and no framework is looked for or downloaded at all.
