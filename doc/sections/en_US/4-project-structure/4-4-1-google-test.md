### Google Test

Every test of the project - the unit and the component ones alike - is written against the [GoogleTest](https://github.com/google/googletest) framework, and the doubles the unit tests are built upon are gmock ones. The GTest framework by itself becomes available by the `FetchContent_Declare`/`FetchContent_MakeAvailable` CMake-commands if no system GTest is available.

Of course, the project's CMakeLists.txt files are probing the GTest only if the tests are enabled - either by the `ENABLE_UNIT_TESTS` or by the `ENABLE_COMPONENT_TESTS` CMake variable, both `OFF` by default. When neither of them is set the whole GTest enabler returns immediately, `enable_testing()` is never called and the library builds without any test dependency at all. The system GTest probe may be turned OFF by setting an appropriate value to the `GTEST_TRY_SYSTEM_PROBE` CMake variable.

The framework version being pulled is the `TEMPLATE_APP_GTEST_GIT_TAG` cache variable, `v1.16.0` by default, taken from the `TEMPLATE_APP_GTEST_GIT` repository.

Look for a [cmake/enablers/template-project-GTest-enabler.cmake](/cmake/enablers/template-project-GTest-enabler.cmake) to see details or change the GTest version etc..

Every test executable links `GTest::gtest_main` and `GTest::gmock`, so none of them carries a `main()` of its own, and registers its cases with CTest through the `gtest_add_tests()` command of the standard `GoogleTest` CMake module with a 600 seconds timeout per case.
