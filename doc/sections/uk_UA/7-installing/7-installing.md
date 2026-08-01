# Встановлення

Для виконання встановлення необхідно виконати команди інсталяції всередині директорії побудови.

## Встановлення в директорію по замовчуванню

Для того щоб встановити побудовану бібліотеку (як показано раніше) у системні директорії по замовчуванню необхідно виконати наступні команди у терміналі (для GNU/Linux):

```
# встановлює побудовану бібліотеку у /usr/local/lib/ наприклад
# і встановлює заголовкові підключаємі файли у
# /usr/local/include/ImagesAnnotatorDataDrivers-0/

sudo cmake --install .
```

Зазвичай цільовою директорією для бібліотеки є `/usr/local/lib` (на Unix-подібних системах), яка не обов'язково знаходиться у типовому шляху пошуку завантажувача твого дистрибутиву - виконай після цього `sudo ldconfig` або встанови у `/usr`, як показано нижче, якщо побудований бінарний файл не знаходиться під час виконання.

## Встановлення у директорію по вибору

Для того щоб встановити бібліотеку у глобально доступну системну директорію необхідно додати параметр `--prefix` до команди, яка показана вище, наприклад:

```
# заміни шлях /usr власним за потреби
# виконувати з директорії побудови проекту

sudo cmake --install . --prefix "/usr"
```

Як альтернатива, будь-який шлях може бути вказаний - локальний префікс у домашній директорії працює так само добре, доки проект-споживач спрямований на нього змінною `CMAKE_PREFIX_PATH`.

## Що саме встановлюється

Для префікса `/usr` з прикладу вище встановлене дерево виглядає так:

```
/usr/lib/libImagesAnnotatorDataDrivers-0.so.0.11.0     бінарний файл бібліотеки
/usr/lib/libImagesAnnotatorDataDrivers-0.so.0          символьне посилання SOVERSION
/usr/lib/libImagesAnnotatorDataDrivers-0.so            символьне посилання для розробки
/usr/lib/cmake/ImagesAnnotatorDataDrivers-0/
    ImagesAnnotatorDataDrivers-0Config.cmake           точка входу для find_package()
    ImagesAnnotatorDataDrivers-0Config-noconfig.cmake
    ImagesAnnotatorDataDrivers-0Targets.cmake          експортована імпортована ціль
    ImagesAnnotatorDataDrivers-0Targets-noconfig.cmake
/usr/include/ImagesAnnotatorDataDrivers-0/*.h          публічні заголовкові файли
```

Файли `*.h` останнього рядка є одинадцятьма публічними заголовковими файлами директорії [src/lib/facade/public](/src/lib/facade/public): `LibraryFacade.h`, `LibraryContext.h`, `ILib.h`, `IAnnotationsDB.h`, `IImagesPathsDBProvider.h`, `IRecordsFactory.h`, `IDBMerger.h`, `ImageRecord.h`, `ImageRecordRect.h`, `IRecord.h` і `IProvider.h`.

Назва `ImagesAnnotatorDataDrivers-0` бінарного файлу, піддиректорії підключаємих файлів і CMake-пакета є значенням `PROJECT_LIBRARY_NAME`, отож усі вони слідують за [налаштованими складовими імені](/doc/sections/uk_UA/5-project-build/compression/5-23-customizing-library-name-segments.md), якщо ти якісь із них сконфігуруєш.

Сам бінарний файл бібліотеки встановлюється як компонент `Runtime`, а символьне посилання для розробки `lib...so` - як компонент `Development`, що і потрібно пакувальнику дистрибутиву для розділення пакунків часу виконання і пакунків для розробки.

Після встановлення спрямуй власний проект на пакет за допомогою `find_package(ImagesAnnotatorDataDrivers-0 REQUIRED)` - див. секцію [Використання бібліотеки у власному проекті](/doc/sections/uk_UA/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) щодо повного перевіреного прикладу.

## Встановлення документації

Якщо проект був сконфігурований для підтримки встановлення згенерованої документації командою, яка може виглядати наступним чином:

```
# з директорії побудови проекту

cmake ../ -DENABLE_DOC_DOXYGEN=ON -DDOXYGEN_DO_INSTALL=ON
```

Команда інсталяції (наприклад, та яка показана у секції [Встановлення в директорію по замовчуванню](#встановлення-в-директорію-по-замовчуванню)) встановить згенеровану HTML документацію у відповідні директорії.
