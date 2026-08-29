## Налаштування складових імені встановлюваної бібліотеки

Імʼя бібліотеки, яке бачать проекти-споживачі (бінарник бібліотеки, субдиректорія
`include/<name>/` для публічних заголовків, директорія CMake package і простір
імен імпортованої цілі що експортується через `install(EXPORT ...)`) виводиться
з трьох опціональних CMake-опцій. Вони дозволяють паралельним встановленням
різних версій бібліотеки співіснувати у системі - наприклад
`include/ImagesAnnotatorDataDrivers-0.12.0-dev/` поруч з
`include/ImagesAnnotatorDataDrivers-0.12/`.

| Опція | За замовчуванням | Ефект |
|---|---|---|
| `-DLIB_INCLUDE_MINOR_IN_NAME=OFF` | `ON` | Прибирає `.<minor>`, який імʼя несе за замовчуванням |
| `-DLIB_INCLUDE_MICRO_IN_NAME=ON` | `OFF` | Додає `.<micro>` (вмикає прапорець minor неявно) |
| `-DLIB_NAME_SUFFIX=-dev` | `""` | Додає довільний завершальний суфікс |

Приклади імен бібліотеки для даного проекту версії `0.12.0`:

| Прапорці конфігурації | Імʼя бібліотеки |
|---|---|
| (немає) | `ImagesAnnotatorDataDrivers-0.12` |
| `-DLIB_INCLUDE_MINOR_IN_NAME=OFF` | `ImagesAnnotatorDataDrivers-0` |
| `-DLIB_INCLUDE_MICRO_IN_NAME=ON` | `ImagesAnnotatorDataDrivers-0.12.0` |
| `-DLIB_NAME_SUFFIX=-dev` | `ImagesAnnotatorDataDrivers-0.12-dev` |
| `-DLIB_INCLUDE_MICRO_IN_NAME=ON -DLIB_NAME_SUFFIX=-dev` | `ImagesAnnotatorDataDrivers-0.12.0-dev` |

Мінорна версія входить до імені навмисно: простір імен публічного API -
`ImagesAnnotatorDataDrivers012` - несе ту саму пару з головного і мінорного
номерів, отож дві мінорні версії встановлюються цілком поруч (окремі бінарники,
окремі директорії заголовків, окремі пакунки CMake) і можуть навіть бути
злінковані в одну й ту саму одиницю трансляції.

Значення за замовчуванням - `ImagesAnnotatorDataDrivers-0.12` - є тим іменем, яке
припускає кожна інша секція даної документації. Змінюй його лише тоді, коли
свідомо бажаєш встановлення пліч-о-пліч, і памʼятай, що яке б імʼя ти не обрав,
саме його має вказувати виклик `find_package()` у твоєму проекті-споживачі.

Приклад поєднаної конфігурації:

```
# всередині кореневої директорії проекту

cmake -S . -B build \
  -DLIB_INCLUDE_MICRO_IN_NAME=ON \
  -DLIB_NAME_SUFFIX=-dev
cmake --build build
cmake --install build --prefix /usr/local
```

Однакове імʼя послідовно використовується для кожного встановлюваного артефакту.
Із залишеними без змін значеннями за замовчуванням і префіксом `/usr/local` це:

- `/usr/local/lib/libImagesAnnotatorDataDrivers-0.12.so.0.12.0` (бінарник спільної бібліотеки, плюс символьне посилання `SOVERSION` `libImagesAnnotatorDataDrivers-0.12.so.0` і символьне посилання імені для лінкувальника `libImagesAnnotatorDataDrivers-0.12.so`),
- `/usr/local/include/ImagesAnnotatorDataDrivers-0.12/*.h` (субдиректорія публічних заголовків, оголошена через `INSTALL_INTERFACE` цілі бібліотеки),
- `/usr/local/<libdir>/cmake/ImagesAnnotatorDataDrivers-0.12/` + імпортована ціль `ImagesAnnotatorDataDrivers-0.12::ImagesAnnotatorDataDrivers-0.12` згенерована через `install(EXPORT ...)` - проекти-споживачі на CMake використовують `find_package(ImagesAnnotatorDataDrivers-0.12 REQUIRED)`. Частина `<libdir>` є тим, у що `GNUInstallDirs` розвʼязує `CMAKE_INSTALL_LIBDIR` у твоїй системі (звичайне `lib` у поширеному випадку).

Зауваж, що простір імен C++ публічного API є цілком окремою річчю і **не**
зазнає впливу цих опцій: він залишається `ImagesAnnotatorDataDrivers012`, з
головним і другорядним номерами версії, вкарбованими у сам ідентифікатор.

Виведення імʼя реалізоване у [cmake/template-project-misc-variables-declare.cmake](/cmake/template-project-misc-variables-declare.cmake) і доступне як CMake cache-змінна `PROJECT_LIBRARY_NAME`, яку використовує решта системи побудови. Запит `LIB_INCLUDE_MICRO_IN_NAME=ON` разом із явним `LIB_INCLUDE_MINOR_IN_NAME=OFF` спричиняє попередження на стадії конфігурування, а прапорець minor вмикається назад неявно щоб згенероване імʼя залишалось синтаксично коректним.
