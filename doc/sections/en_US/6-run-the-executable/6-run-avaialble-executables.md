# Run the available executables

Be honest about what is here: this project produces a shared library and no application binary at all, so there is nothing of the project's own to start. The executables that do get built are the GoogleTest binaries - the `UTEST_*` unit tests and the `CTEST_*` component tests - and they only appear when the tests were enabled during the configure stage (see the [Enabling unit testing](/doc/sections/en_US/5-project-build/testing/5-3-1-enabling-unit-testing.md) subsection). Everything else you might want to run is a program of your own, linked against the installed library.

1. [IDE run](/doc/sections/en_US/6-run-the-executable/6-1-IDE-run.md)
1. Command line run
    1. [Searching for the generated library and test binaries](/doc/sections/en_US/6-run-the-executable/6-2-1-searching-for-the-generated-executable.md)
    1. [Starting the test binaries and a program linked against the library](/doc/sections/en_US/6-run-the-executable/6-2-2-starting-the-generated-executable.md)
1. Tests run
    1. [Run tests by the ctest](/doc/sections/en_US/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md)
    1. [Manual tests run](/doc/sections/en_US/6-run-the-executable/6-3-2-manual-tests-run.md)
