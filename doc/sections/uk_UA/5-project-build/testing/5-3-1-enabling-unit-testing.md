### Вмикання юніт-тестів

Для того щоб зробити юніт тестування доступним (для побудови і запуску) необхідно перекорфігурувати проект з увімкненою опцією `ENABLE_UNIT_TESTS` за допомогою команди (для GNU/Linux):

```
# у кореневій директорії проекту

mkdir -vp build && cd build && cmake ../ -DENABLE_UNIT_TESTS=ON && cmake --build . --target all
```

Юніт-тести **не** лінкують побудовану динамічну бібліотеку. Вони компілюють ті самі одиниці трансляції безпосередньо з дерева проекту і підставляють gmock-замінники, які знаходяться під [src/tests/mocks](/src/tests/mocks), замість сусідніх компонентів, що і дозволяє їм досліджувати окремий клас в ізоляції.

Створюються наступні виконувані файли юніт-тестів:

| Ціль | Що покриває |
|---|---|
| `UTEST_LibraryFacade` | статичні вхідні двері публічного API |
| `UTEST_LibFactory` | фабрику позаду фасаду |
| `UTEST_LibMain` | точку входу одного пострілу `libcall(ctx)` |
| `UTEST_ImageRecord` | окремий запис анотованого зображення |
| `UTEST_ImageRecordRect` | окремий іменований прямокутник запису |
| `UTEST_RecordsFactory` | реалізацію фабрики записів |
| `UTEST_AnnotationsJSONSerializator` | JSON-серіалізатор файлу проекту |
| `UTEST_DefaultDBMerger` | типовий об'єкт злиття баз даних |
| `UTEST_ImageRecords2JSONConverter` | записи зображень -> JSON |
| `UTEST_JSON2ImageRecordsConverter` | JSON -> записи зображень |
| `UTEST_ImageRecordJSON2RecordConverter` | окремий елемент JSON -> запис зображення |
| `UTEST_ImageRecordRectJSON2RecordConverter` | окремий елемент JSON -> прямокутник |

Існує другий, незалежний перемикач для компонентних тестів - `ENABLE_COMPONENT_TESTS`. Ті йдуть навпаки: `CTEST_AnnotationsDirDB` лінкує справжню побудовану динамічну бібліотеку і працює з нею виключно через встановлювані публічні заголовкові файли, без жодних замінників і без доступу до нутрощів, точно так, як це робив би проект-споживач. `CTEST_SimpleLogger` покриває логер, вкомпільований у бібліотеку.

Обидва перемикачі за замовчуванням мають значення `OFF` і чудово уживаються разом - увімкни обидва, щоб отримати повний набір тестів:

```
# з кореневої директорії проекту

cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

Будь-якої з двох опцій достатньо, щоб запустити пошук фреймворку GoogleTest і виклик `enable_testing()`, див. підрозділ [Запобігання використання GTest з ОС](/doc/sections/uk_UA/5-project-build/testing/5-3-2-disabling-system-GTest-probe.md) щодо подробиць про те, звідки береться фреймворк. Про те, як запускати побудоване, читай у підрозділах [Запуск тестів за допомогою ctest](/doc/sections/uk_UA/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md) і [Ручний запуск тестів](/doc/sections/uk_UA/6-run-the-executable/6-3-2-manual-tests-run.md).
