# Forking and replacing the origin

If you are going to contribute to the library, or to maintain a derivative of it, do not push into the upstream repository - fork it first and let your local clone point at your own fork instead.

Forking itself may be performed in the git Web interface panel (the **Fork** button at [GitHub](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers) for example).

Having your own repository created, clone this project into your local directory (see the [Cloning the project](/doc/sections/en_US/1-cloning-the-project/1-cloning-the-project.md) section) and perform the origin replacement command from inside the project root directory:

```
# REPLACE next URL with yours
git remote set-url origin https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
```

Replace the `https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git` URL with one of your newly created project repository URL.

To verify successful repository remote URL (where all the changes will be pushed) replacement execute next command:

```
git remote -v
```

Which should show your new project repository.

Finally push your repository to a new location by executing next command:

```
git push
```

Should you prefer to keep the upstream repository reachable for the future updates, add it as a second remote instead of dropping it:

```
git remote add upstream https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
git fetch upstream
```
