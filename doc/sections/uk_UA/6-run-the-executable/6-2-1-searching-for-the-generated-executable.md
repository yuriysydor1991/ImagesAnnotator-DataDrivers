### Пошук згенерованої бібліотеки і бінарних файлів тестів

Даний проект не будує жодного власного виконуваного файлу, отож шукати програму з назвою проекту немає сенсу. Побудова створює динамічну бібліотеку і, якщо тести було увімкнено на стадії конфігурації, бінарні файли тестів.

Для того щоб знайти побудовану бібліотеку у директорії побудови проекту необхідно виконати команду `find`:

```
# з директорії побудови проекту

find . -name 'libImagesAnnotatorDataDrivers-0.11.so*'
```

Що за типових параметрів конфігурації дає три записи, і усі вони знаходяться у піддиректорії `src/lib` дерева побудови:

```
./src/lib/libImagesAnnotatorDataDrivers-0.11.so.0.11.0    справжній бінарний файл бібліотеки
./src/lib/libImagesAnnotatorDataDrivers-0.11.so.0         символьне посилання SONAME
./src/lib/libImagesAnnotatorDataDrivers-0.11.so           символьне посилання для лінкувальника
```

Частина `ImagesAnnotatorDataDrivers-0.11` є назвою бібліотеки, похідною від назви проекту разом з його головним номером версії. Заміни її власною, якщо змінюєш назву проекту у кореневому файлі `CMakeLists.txt` (змінна `CMAKE_PROJECT_NAME`) або якщо налаштовуєш складові імені параметрами `LIB_INCLUDE_MINOR_IN_NAME` / `LIB_INCLUDE_MICRO_IN_NAME` / `LIB_NAME_SUFFIX` - див. підрозділ [Налаштування складових імені встановлюваної бібліотеки](/doc/sections/uk_UA/5-project-build/compression/5-23-customizing-library-name-segments.md). Стадія конфігурації друкує чинну назву рядком стану `PROJECT_LIBRARY_NAME: ...`, отож у разі сумніву просто зазирни туди.

Для того щоб знайти скомпільовані бінарні файли тестів, шукай за шаблонами `UTEST_*` (юніт-тести) і `CTEST_*` (компонентні тести):

```
# з кореневої директорії проекту або з директорії побудови

find -type f -executable -name 'UTEST_*' -o -type f -executable -name 'CTEST_*'
```

Вони розкидані по дереву побудови, кожен поруч з вихідними кодами, які він перевіряє, наприклад:

```
./src/lib/facade/tests/unit/LibraryFacade/UTEST_LibraryFacade
./src/records/tests/unit/ImageRecord/UTEST_ImageRecord
./src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger/UTEST_DefaultDBMerger
./src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB/CTEST_AnnotationsDirDB
```

Якщо команда `find` взагалі нічого не повертає, то тести найімовірніше не було увімкнено на стадії конфігурації - див. підрозділ [Вмикання юніт-тестів](/doc/sections/uk_UA/5-project-build/testing/5-3-1-enabling-unit-testing.md).
