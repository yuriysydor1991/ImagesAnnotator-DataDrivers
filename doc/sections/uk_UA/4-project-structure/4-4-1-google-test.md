### Фреймворк тестів Google Test

Кожен тест проекту - як модульний, так і компонентний - написаний на основі фреймворку [GoogleTest](https://github.com/google/googletest), а дублери, на яких побудовані модульні тести, є gmock-івськими. Сам фреймворк GTest стає доступним за допомогою CMake-команд `FetchContent_Declare`/`FetchContent_MakeAvailable`, якщо системний GTest недоступний.

Звісно, файли `CMakeLists.txt` проекту визначають наявність GTest тільки якщо тести увімкнені - або CMake-змінною `IADD_ENABLE_UNIT_TESTS`, або `IADD_ENABLE_COMPONENT_TESTS`, обидві з яких за замовчуванням є `OFF`. Коли жодна з них не встановлена, увесь активатор GTest негайно повертається, `enable_testing()` ніколи не викликається і бібліотека будується взагалі без жодної тестової залежності. Використання системного GTest можна вимкнути встановленням відповідного значення для CMake-змінної `IADD_GTEST_TRY_SYSTEM_PROBE`.

Версія фреймворку, що завантажується, задається змінною кешу `IADD_GTEST_GIT_TAG`, за замовчуванням `v1.16.0`, і береться з репозиторію `IADD_GTEST_GIT`.

Переглянь файл [cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake](/cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake), щоб побачити подробиці або змінити версію GTest тощо.

Кожен виконуваний файл тесту лінкує `GTest::gtest_main` і `GTest::gmock`, тому жоден з них не несе власного `main()`, і реєструє свої випадки у CTest командою `gtest_add_tests()` зі стандартного CMake-модуля `GoogleTest` із обмеженням часу 600 секунд на випадок.
