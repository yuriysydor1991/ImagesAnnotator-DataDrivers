## IDE build

Of course if your IDE supports CMake build system integration you may just press the `Build` button somewhere in the IDE window with opened project and it's done! No need to perform a search and execute commands.

Keep in mind though that this project produces a shared library and, optionally, the test executables - there is no application binary of its own here, so the IDE's `Run` button has nothing to start by default. If you want it to do something useful, configure the project with the tests enabled (see the [Enabling unit testing](/doc/sections/en_US/5-project-build/testing/5-3-1-enabling-unit-testing.md) subsection) and point the IDE at one of the produced `UTEST_*` or `CTEST_*` binaries.
