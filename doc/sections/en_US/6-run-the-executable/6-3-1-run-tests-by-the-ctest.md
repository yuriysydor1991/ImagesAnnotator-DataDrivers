### Run tests by the ctest

If enabled by the developer through the `ENABLE_UNIT_TESTS` and/or the `ENABLE_COMPONENT_TESTS` variables and successfully build, the one may run all available test by the `ctest` command from a project build directory (GNU/Linux based):

```
# run from the project build directory

ctest
```

Add the `--output-on-failure` flag to get the GoogleTest output of the cases that did not pass printed right away instead of a bare list of names:

```
# run from the project build directory

ctest --output-on-failure
```

Alternatively, run the `ctest` command from any location by specifying the test dir (GNU/Linux based):

```
ctest --tests-dir /path/to/the/project/build/directory
```

In order to run particular test execute the `ctest` command with test's name after the `-R` command line flag. For example, for the `UTEST_IADataDriversFacade` test it'll look something like this:

```
# from the project build directory 

ctest -R UTEST_IADataDriversFacade
```

The `-R` flag takes a regular expression matched against the registered case name, and the cases here are registered in a `<binary name>.<case>.<arguments>` shape, so the same flag narrows things down as far as you like - a whole binary, or one single case:

```
# every component test of the annotations directory database
ctest -R CTEST_AnnotationsDirDB

# one particular case
ctest -R UTEST_DefaultDBMerger.appends_new_records_and_skips_duplicates
```

Use `ctest -N` to list what is registered without running anything.

The [debug-test.sh](/scripts/build/debug-test.sh) script of the [Quick build scripts](/doc/sections/en_US/5-project-build/5-37-quick-build-scripts.md) section performs the very same `ctest` call inside the `build/debug` directory, either on its own or as a step of the `Debug` build scripts, whenever the `--test` parameter is given (GNU/Linux based):

```
# inside the project root directory

scripts/build/debug-test.sh --test
```
