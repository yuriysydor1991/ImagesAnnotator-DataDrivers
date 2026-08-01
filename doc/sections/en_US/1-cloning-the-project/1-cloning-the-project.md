# Cloning the project

In order to build the library from the sources clone this project into your local directory by executing next command in the terminal (GNU/Linux based):

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
```

The library lives on the repository default `main` branch, so no `--branch` flag is required. Should you prefer to be explicit about it anyway:

```
git clone --branch main https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
```

After this command executes you should find a directory named `ImagesAnnotator-DataDrivers` inside your current one. So enter it by calling `cd ImagesAnnotator-DataDrivers`.

The sources of this library were extracted out of the [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) application - namely out of its `src/annotator-business/dbs/AnnotationsDBs` annotations project JSON database component - so that any project willing to read or write the very same annotations project files may reuse them instead of carrying its own copy. The application itself is going to drop its own copy and consume this library instead.

Note that the dataset exporters of that application (the plain text, YOLO and PyTorch-Vision ones) are **not** a part of this library: they are to become a separate project of their own.

Once the sources are in place proceed with the [Requirements](/doc/sections/en_US/3-requirements/3-requirements.md) and then with the [Project build](/doc/sections/en_US/5-project-build/5-project-build.md) sections.

If you do not intend to hack on the library itself and only need to consume it, there is no obligation to keep the sources around: configure, build and install them once (see the [Installing](/doc/sections/en_US/7-installing/7-installing.md) section) and then point your own project at the installed CMake package.
