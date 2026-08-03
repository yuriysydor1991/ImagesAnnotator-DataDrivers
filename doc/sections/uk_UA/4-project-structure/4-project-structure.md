# Структура проекту

Репозиторій утримує єдиний CMake-проект із назвою `ImagesAnnotatorDataDrivers`, який виробляє рівно один двійковий артефакт - динамічну бібліотеку `ImagesAnnotatorDataDrivers-0.11` - разом із її публічними заголовковими файлами, придатними до встановлення, і файлами її CMake-пакунку. Жодного виконуваного файлу тут не генерується.

Погляд на дерево проекту з висоти пташиного польоту:

```
.
├── CMakeLists.txt     кореневе оголошення project(): назва, версія, стандарт C++17
├── cmake/             уся CMake-інфраструктура: активатори залежностей, опції
│                      компіляції, пакувальники і цілі аналізаторів коду
├── doc/               дана документація, діаграми і налаштування Doxygen
├── misc/              допоміжні файли CI: Jenkinsfile, Dockerfile-и, дані пакувальників
└── src/               вихідні коди
```

І сама директорія `src/`, де розміщені усі цікаві частини:

```
src/
├── project-global-decls.h.in            сконфігуровані константи часу компіляції
├── lib/
│   ├── cmake/                           генерація CMake-пакунку для встановлення
│   ├── facade/
│   │   ├── public/                      публічні заголовкові файли для встановлення -
│   │   │                                уся поверхня API
│   │   ├── LibraryFacade.cpp            реалізація статичних вхідних дверей
│   │   └── LibraryContext.cpp
│   └── libmain/
│       ├── LibMain.{h,cpp}              типова реалізація ILib            простір імен lib0impl
│       └── LibFactory.{h,cpp}           фабрика реалізацій                простір імен lib0impl
├── records/                             ImageRecord, ImageRecordRect, RecordsFactory
│                                                        простір імен iannotator::records
├── helpers/                             IHelper, SortHelper, UrlHelper,
│                                        ImageRecordUrlAndPathHelper    простір імен helpers
├── dbs/AnnotationsDBs/                  AnnotationsDirDB, AnnotationsJSONSerializator,
│   │                                    AnnotationsDBTypes
│   │                                        простір імен iannotator::dbs::annotations
│   ├── converters/                      перетворювачі з JSON у записи і назад
│   └── mergers/                         DefaultDBMerger
├── log/                                 компонент журналювання
└── tests/mocks/                         спільні gmock-замінники
```

Кілька речей варто знати про дану структуру:

- Усе, що знаходиться у [src/lib/facade/public](/src/lib/facade/public), встановлюється і є єдиним, що коли-небудь включає споживач. Кожне оголошення там належить до простору імен `ImagesAnnotatorDataDrivers011`, назва якого навмисно несе мажорну і мінорну версії бібліотеки - див. [Публічні інтерфейсні файли бібліотеки](/doc/sections/uk_UA/4-project-structure/4-8-the-librarys-installable-include-header-files.md).
- Усе інше під [src](/src) є реалізацією: [src/lib/libmain](/src/lib/libmain) утримує пару вхідних класів, а компоненти, якими вона керує, лежать поруч як [src/records](/src/records), [src/helpers](/src/helpers) і [src/dbs](/src/dbs). Вона живе у власних просторах імен (`lib0impl`, `iannotator::records`, `iannotator::dbs::annotations`, `helpers`), жоден з її заголовкових файлів не встановлюється і ніщо поза двійковим файлом бібліотеки не здатне до неї дотягнутись.
- Компонент журналювання [src/log](/src/log) компілюється прямо у вироблену динамічну бібліотеку (його `default-logger` долучається як об'єктна бібліотека), тому споживачеві не потрібно постачати жодної власної реалізації журналювання, щоб злінкуватись із бібліотекою.
- Вихідні коди тестів розміщені поруч із кодом, який вони покривають, всередині піддиректорій `tests/unit` і `tests/component` кожного компоненту, а спільні для них замінники (mocks) живуть у [src/tests/mocks](/src/tests/mocks). Див. підрозділ [Тести проекту](/doc/sections/uk_UA/4-project-structure/4-4-project-tests.md).

Наступні підрозділи проходяться по частинах проекту детальніше:

1. [Діаграми проекту](/doc/sections/uk_UA/4-project-structure/4-0-project-diagrams.md)
1. [Де розміщена реалізація](/doc/sections/uk_UA/4-project-structure/4-1-implement-code-straight-away.md)
1. [Публічні інтерфейсні файли бібліотеки](/doc/sections/uk_UA/4-project-structure/4-8-the-librarys-installable-include-header-files.md)
1. [API бази даних анотацій](/doc/sections/uk_UA/4-project-structure/4-9-the-annotations-database-api.md)
1. [JSON-формат проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md)
1. [Зміна назви проекту і назви бібліотеки](/doc/sections/uk_UA/4-project-structure/4-2-changing-the-project-and-executable-name.md)
1. [Версіювання і інші параметри проекту](/doc/sections/uk_UA/4-project-structure/4-3-version-tracking-and-other-project-parameters.md)
1. [Мінімально можливі версії](/doc/sections/uk_UA/4-project-structure/4-6-minimal-possible-versions.md)
1. [Тести проекту](/doc/sections/uk_UA/4-project-structure/4-4-project-tests.md)
    1. [Фреймворк тестів Google Test](/doc/sections/uk_UA/4-project-structure/4-4-1-google-test.md)
1. [Розширення](/doc/sections/uk_UA/4-project-structure/4-5-extensions.md)
