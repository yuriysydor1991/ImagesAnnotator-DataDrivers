### Запобігання використання GTest з ОС

Для вимкнення використання ресурсів наявного фреймворку GTest необхідно встановити відповідне значення для CMake-змінної `IADD_GTEST_TRY_SYSTEM_PROBE` за допомогою команди конфігурування проекту (для GNU/Linux):

```
cmake -B build -S . -DIADD_ENABLE_UNIT_TESTS=ON -DIADD_GTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

Під час виконання команди система побудови проекту спробує встановити GTest тільки у межах поточного проекту через мережу Інтернет з версією вказаною у файлі [cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake](/cmake/enablers/imagesannotator-datadrivers-GTest-enabler.cmake). Репозиторій і тег утримуються кеш-змінними `IADD_GTEST_GIT` і `IADD_GTEST_GIT_TAG`, останній за замовчуванням закріплений на `v1.16.0`.

Той самий перемикач стосується і компонентних тестів, отож якщо ти увімкнув їх замість (або разом з) юніт-тестів, команда буде такою:

```
cmake -B build -S . -DIADD_ENABLE_UNIT_TESTS=ON -DIADD_ENABLE_COMPONENT_TESTS=ON -DIADD_GTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

Зауваж, що увесь модуль вмикання GTest завершується достроково, доки хоча б одна з опцій `IADD_ENABLE_UNIT_TESTS` / `IADD_ENABLE_COMPONENT_TESTS` не має значення `ON`, отож з вимкненими тестами значення `IADD_GTEST_TRY_SYSTEM_PROBE` нічого не змінює і жоден фреймворк взагалі не шукається і не завантажується.
