## Тести проекту

Бібліотека покрита тестами, які тримаються поруч із кодом, що вони випробовують: кожна директорія компоненту має власну піддиректорію `tests/unit` і, де це має сенс, `tests/component`. Обидва види вимкнені за замовчуванням і вмикаються кожен своєю окремою CMake-опцією, тому побудова самої бібліотеки ніколи не вимагає GoogleTest.

```
# з кореня проекту - обидва види одразу

cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

### Модульні тести проти компонентних

Ці два види відрізняються тим, що саме вони тестують, і ця різниця видима у їхніх файлах `CMakeLists.txt`:

- **Модульні тести** (`-DENABLE_UNIT_TESTS=ON`, назви цілей із префіксом `UTEST_`) взагалі не лінкують вироблену динамічну бібліотеку. Кожен такий виконуваний файл називає саме ті одиниці трансляції компоненту, що тестується, і компілює їх прямо з дерева проекту, тоді як усі його співробітники замінені gmock-замінниками з [src/tests/mocks](/src/tests/mocks) (журналювання, `project-global-decls.h`, `LibraryContext` і `LibMain`) або замінниками окремого компоненту, що зберігаються у власній директорії `tests/mocks` того компоненту. Це дає тестам доступ до внутрішніх заголовкових файлів і дозволяє їм ізолювати єдиний клас.
- **Компонентні тести** (`-DENABLE_COMPONENT_TESTS=ON`, назви цілей із префіксом `CTEST_`) роблять протилежне: `CTEST_AnnotationsDirDB` лінкує справжню динамічну бібліотеку `ImagesAnnotatorDataDrivers-0.11` і дотягується до неї виключно через встановлювані публічні заголовкові файли - жодного замінника і жодного внутрішнього заголовкового файлу немає на його шляху включень. Отже, це тест бібліотеки рівно таким, яким її бачить споживач, який керує справжніми JSON-файлами проектів, узятими з його директорії `ctest_data`.

### Цілі тестів

| Ціль | Вид | Розміщення |
|---|---|---|
| `UTEST_LibraryFacade` | модульний | [src/lib/facade/tests/unit/LibraryFacade](/src/lib/facade/tests/unit/LibraryFacade) |
| `UTEST_LibFactory` | модульний | [src/lib/libmain/tests/unit/LibFactory](/src/lib/libmain/tests/unit/LibFactory) |
| `UTEST_LibMain` | модульний | [src/lib/libmain/tests/unit/LibMain](/src/lib/libmain/tests/unit/LibMain) |
| `UTEST_ImageRecord` | модульний | [src/records/tests/unit/ImageRecord](/src/records/tests/unit/ImageRecord) |
| `UTEST_ImageRecordRect` | модульний | [src/records/tests/unit/ImageRecordRect](/src/records/tests/unit/ImageRecordRect) |
| `UTEST_RecordsFactory` | модульний | [src/records/tests/unit/RecordsFactory](/src/records/tests/unit/RecordsFactory) |
| `UTEST_AnnotationsJSONSerializator` | модульний | [src/dbs/AnnotationsDBs/tests/unit/AnnotationsJSONSerializator](/src/dbs/AnnotationsDBs/tests/unit/AnnotationsJSONSerializator) |
| `UTEST_LibraryImpl` | модульний | [src/dbs/AnnotationsDBs/tests/unit/LibraryImpl](/src/dbs/AnnotationsDBs/tests/unit/LibraryImpl) |
| `UTEST_DefaultDBMerger` | модульний | [src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger](/src/dbs/AnnotationsDBs/tests/unit/mergers/DefaultDBMerger) |
| `UTEST_ImageRecords2JSONConverter` | модульний | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecords2JSONConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecords2JSONConverter) |
| `UTEST_JSON2ImageRecordsConverter` | модульний | [src/dbs/AnnotationsDBs/tests/unit/converters/JSON2ImageRecordsConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/JSON2ImageRecordsConverter) |
| `UTEST_ImageRecordJSON2RecordConverter` | модульний | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordJSON2RecordConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordJSON2RecordConverter) |
| `UTEST_ImageRecordRectJSON2RecordConverter` | модульний | [src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordRectJSON2RecordConverter](/src/dbs/AnnotationsDBs/tests/unit/converters/ImageRecordRectJSON2RecordConverter) |
| `CTEST_AnnotationsDirDB` | компонентний | [src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB) |
| `CTEST_log` | компонентний | [src/log/tests/component/log](/src/log/tests/component/log) |
| `CTEST_DefaultLogger` | компонентний | [src/log/default-logger/tests/component/DefaultLogger](/src/log/default-logger/tests/component/DefaultLogger) |

Кожен сконфігурований виконуваний файл реєструє свої випадки у CTest командою `gtest_add_tests()`, тому `ctest -R UTEST_DefaultDBMerger` і подібні працюють як очікується. Див. підрозділ [Запуск тестів за допомогою ctest](/doc/sections/uk_UA/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md) щодо способів їх запуску і розділ [Побудова проекту](/doc/sections/uk_UA/5-project-build/5-project-build.md) щодо способів їх увімкнення.
