## IDE run

Of course, if your IDE supports CMake build system integration you may just press the `Build` button somewhere in the IDE window with opened project and it's done! No need to perform a search and execute commands.

The `Run` button, however, needs a target to run, and this project builds no executable of its own - only the shared library. Configure the project with the tests enabled (see the [Enabling unit testing](/doc/sections/en_US/5-project-build/testing/5-3-1-enabling-unit-testing.md) subsection) and the `UTEST_*` and `CTEST_*` binaries will show up in the IDE's run target list, ready to be started and debugged like any other program. Most CMake aware IDEs also expose the `ctest` integration, which runs the whole suite at once.
