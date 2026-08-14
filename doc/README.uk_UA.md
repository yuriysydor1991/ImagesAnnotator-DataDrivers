**Бібліотека бази даних анотацій проекту ImagesAnnotator**

# Що це таке

`ImagesAnnotatorDataDrivers` - це динамічна бібліотека на C++17, яка зчитує і записує файли проектів анотацій застосунку [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git) - JSON-документи, які зберігають анотовані зображення, їхні розміри і іменовані прямокутники, намальовані поверх них.

Раніше цей код знаходився всередині самого застосунку. Його було винесено в окрему бібліотеку для того, щоб застосунок ImagesAnnotator і **будь-який інший інструмент** могли читати і записувати один і той самий формат файлу проекту без дублювання розбирача, серіалізатора і правил злиття.

Усе, до чого торкається проект-споживач, приховано за абстрактними інтерфейсами заголовкових підключаємих файлів [src/lib/facade/public](/src/lib/facade/public), отож ані класи реалізації, ані сторонні залежності бібліотеки не потрапляють у код проекту-споживача.

Більше за посиланням [kytok.org.ua](http://www.kytok.org.ua/)

💵 Підтримай проект за посиланням [http://kytok.org.ua/page/pozertvy](http://kytok.org.ua/page/pozertvy)

# Можливості

- **Завантаження і збереження JSON-файлу проекту анотацій** - `load_db()` розбирає файл проекту у пам'ять, а `store_db()` записує його назад або за тим самим шляхом, або за новим.
- **Набір записів зображень у пам'яті** - кожне анотоване зображення є записом `ImageRecord`, який містить власну вихідну директорію (або URL веб-сторінки), відносний шлях, власні ширину і висоту, масштаб перегляду і іменовані прямокутники `ImageRecordRect`.
- **Відстеження змін** - `changed()` повідомляє, чи записи у пам'яті все ще відповідають збереженим, отож застосунок може пропустити запис або запитати користувача про незбережені дані.
- **Злиття з урахуванням дублікатів** - `add_images_db()` додає інший набір записів через `IDBMerger`, пропускаючи вже наявні записи і зберігаючи результат відсортованим. Записи, розміщені у мережі, розбираються через URL API бібліотеки libcurl, отож одне й те саме зображення, вказане двома по-різному записаними URL, розпізнається як одне.
- **Збирання назв анотацій** - `get_available_annotations()` повертає відсортовану множину усіх унікальних назв анотацій (класів), які використовуються у всій базі даних.
- **Видалення запису** - `delete_image_record()` видаляє запис за його повним шляхом.
- **Підмінні фабрика записів і об'єкт злиття** - розмісти власного нащадка `IRecordsFactory` і/або `IDBMerger` у `LibraryContext` і десеріалізація створюватиме твої збагачені підкласи записів і зливатиме їх за твоїми власними правилами.
- **Налаштовувана ідентичність кореневого елемента JSON** - файл проекту має кореневий елемент, названий за іменем *застосунку*, а не бібліотеки, отож назва і версія залишаються налаштовуваними як під час побудови, так і під час виконання.

# Приклад використання

Бібліотека використовується через CMake-пакет. Наступна програма була скомпільована, злінкована і запущена зі встановленою бібліотекою:

```cmake
cmake_minimum_required(VERSION 3.13)
project(MyTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ImagesAnnotatorDataDrivers-0.11 REQUIRED)

add_executable(mytool main.cpp)
target_link_libraries(mytool ImagesAnnotatorDataDrivers-0.11::ImagesAnnotatorDataDrivers-0.11)
```

```cpp
#include <iostream>
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

int main(int argc, char** argv)
{
  if (argc < 2) { return 1; }

  auto db = iadd::LibraryFacade::open_annotations_db(argv[1]);

  if (db == nullptr) { return 1; }

  for (const auto& ir : db->get_images_db()) {
    std::cout << ir->get_full_path() << " rects=" << ir->rects.size() << "\n";
  }

  return db->changed() ? (db->store_db() ? 0 : 1) : 0;
}
```

Назва простору імен `ImagesAnnotatorDataDrivers011` навмисне містить головний і другорядний номери версії бібліотеки: дві версії бібліотеки можуть співіснувати в межах однієї одиниці трансляції без жодного конфлікту символів. Створюй для нього псевдонім, як показано вище, і підняття версії залишатиметься зміною в один рядок на твоєму боці.

Для встановленої бібліотеки працюють як `#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>`, так і просте `#include <LibraryFacade.h>`, оскільки бібліотека експортує і корінь директорії підключаємих файлів, і її версіоновану піддиректорію. Рекомендованою є форма з префіксом: назви заголовкових файлів на кшталт `ImageRecord.h` чи `IRecord.h` є достатньо загальними, щоб призвести до конфлікту у завантаженому шляху пошуку підключаємих файлів.

Більше про API і про сам формат файлу у секціях [API бази даних анотацій](/doc/sections/uk_UA/4-project-structure/4-9-the-annotations-database-api.md), [формат JSON-файлу проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md) і [використання бібліотеки у власному проекті](/doc/sections/uk_UA/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md).

# Залежності

Обидві наступні залежності є **обов'язковими** для побудови бібліотеки і обидві мають значення `ON` за замовчуванням:

| CMake-параметр | Бібліотека | Для чого потрібна |
| --- | --- | --- |
| `ENABLE_NLOHMANN_JSON` | [nlohmann/json](https://github.com/nlohmann/json) | файл проекту анотацій є JSON-документом |
| `ENABLE_LIBCURL` | [libcurl](https://curl.se/libcurl/) | її URL API `CURLU` розбирає і усуває дублікати записів зображень, які вказують на веб-URL замість локального файлу |

Кожна з них спершу шукається у системі і, якщо її не знайдено, завантажується засобами CMake `FetchContent` (nlohmann json на тезі `v3.11.3`). Дивись секції [вмикання інтеграції бібліотеки nlohmann JSON](/doc/sections/uk_UA/5-project-build/5-18-enabling-the-nlohmann-json-library.md) і [вмикання інтеграції libcurl](/doc/sections/uk_UA/5-project-build/5-14-enabling-libcurl.md) щодо змінних перевизначення джерела і тегу.

Обидві лінкуються **приватно**: жоден публічний заголовковий файл не виставляє назовні тип з nlohmann чи curl, отож проект-споживач не успадковує жодної з них і не потребує жодної з них у своєму шляху пошуку підключаємих файлів. Компонент журналювання також вкомпільовано безпосередньо у динамічну бібліотеку, отож споживач не мусить надавати жодної реалізації логера.

# Побудова і тестування

Звичайна побудова:

```
cmake -S . -B build
cmake --build build -j$(nproc)
```

Тести вимкнені за замовчуванням. Для їх побудови і запуску:

```
cmake -S . -B build -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

Цілі `ENABLE_UNIT_TESTS` компілюються безпосередньо з вихідних кодів разом з gmock-замінниками з [src/tests/mocks](/src/tests/mocks), тоді як ціль `CTEST_AnnotationsDirDB` з `ENABLE_COMPONENT_TESTS` лінкує справжню динамічну бібліотеку і працює з нею виключно через публічні заголовкові файли - точно так, як це робить проект-споживач.

Встановлення виконується звичайним `sudo cmake --install build` і детально описане у секції [встановлення](/doc/sections/uk_UA/7-installing/7-installing.md).

# Звідки походить код

Вихідні коди було винесено із застосунку [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git), а саме з його компоненту `src/annotator-business/dbs/AnnotationsDBs`. Застосунок має позбутися власної копії цього коду і використовувати натомість дану бібліотеку.

Експортери наборів даних того застосунку (у простий текст, YOLO v4 і PyTorch-Vision) **не** є частиною даної бібліотеки: вони мають стати окремим власним проектом.

Перегляньте директорію `doc` щодо можливих перекладів поточного md-документа:
- `en_US` за відносною адресою README.md

# Зміст документації

**Даний документ у процесі покращення**

1. [Клонування проекту](/doc/sections/uk_UA/1-cloning-the-project/1-cloning-the-project.md)
1. [Створення форку і заміна оригінального репозиторію](/doc/sections/uk_UA/2-forking-and-replacing-the-origin/2-forking-and-replacing-the-origin.md)
1. [Вимоги](/doc/sections/uk_UA/3-requirements/3-requirements.md)
    1. [Обов'язкові інструменти для ОС на базі GNU/Лінукс](/doc/sections/uk_UA/3-requirements/3-1-required-tools-for-the-GNU-Linux-based-OS.md)
    1. [Обов'язкові інструменти для ОС на базі MS Windows](/doc/sections/uk_UA/3-requirements/3-2-required-tools-for-the-MS-Windows-based-OS.md)
    1. [Необов'язкові пакети для тестів](/doc/sections/uk_UA/3-requirements/3-3-optional-for-the-tests.md)
    1. [Необов'язкові пакети для створення документації](/doc/sections/uk_UA/3-requirements/3-4-optional-for-the-documentation.md)
    1. [Необов'язкові пакети для форматування коду](/doc/sections/uk_UA/3-requirements/3-5-optional-for-the-code-formatting.md)
    1. [Необов'язкові пакети для статичного аналізатора коду cppcheck](/doc/sections/uk_UA/3-requirements/3-6-optional-for-the-code-analyzer-cppcheck.md)
    1. [Необов'язкові пакети для статичного аналізатора коду clang-tidy](/doc/sections/uk_UA/3-requirements/3-7-optional-for-the-code-analyzer-with-clang-tidy.md)
    1. [Необов'язкові пакети для перевірки пам'яті за допомогою Valgrind](/doc/sections/uk_UA/3-requirements/3-8-optional-for-the-memory-checkwith-Valgrind.md)
    1. [Необов'язкові пакети для пакувальника flatpak](/doc/sections/uk_UA/3-requirements/3-9-optional-for-the-flatpak-packager.md)
    1. [Необов'язкові пакети для запусків у docker-контейнерах](/doc/sections/uk_UA/3-requirements/3-10-optional-for-docker-container-runs.md)
    1. [Необов'язкові пакети для пакувальника snap](/doc/sections/uk_UA/3-requirements/3-11-optional-for-snap-packager.md)
1. [Структура проекту](/doc/sections/uk_UA/4-project-structure/4-project-structure.md)
    1. [Діаграми проекту](/doc/sections/uk_UA/4-project-structure/4-0-project-diagrams.md)
    1. [Реалізуй код одразу!](/doc/sections/uk_UA/4-project-structure/4-1-implement-code-straight-away.md)
    1. [Публічні інтерфейсні файли бібліотеки](/doc/sections/uk_UA/4-project-structure/4-8-the-librarys-installable-include-header-files.md)
    1. [API бази даних анотацій](/doc/sections/uk_UA/4-project-structure/4-9-the-annotations-database-api.md)
    1. [Формат JSON-файлу проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md)
    1. [Зміна назви проекту і головного виконуваного файлу](/doc/sections/uk_UA/4-project-structure/4-2-changing-the-project-and-executable-name.md)
    1. [Версіювання і інші параметри проекту](/doc/sections/uk_UA/4-project-structure/4-3-version-tracking-and-other-project-parameters.md)
    1. [Мінімально можливі версії](/doc/sections/uk_UA/4-project-structure/4-6-minimal-possible-versions.md)
    1. [Тести проекту](/doc/sections/uk_UA/4-project-structure/4-4-project-tests.md)
        1. [Фреймворк тестів Google Test](/doc/sections/uk_UA/4-project-structure/4-4-1-google-test.md)
    1. [Розширення](/doc/sections/uk_UA/4-project-structure/4-5-extensions.md)
1. [Побудова проекту](/doc/sections/uk_UA/5-project-build/5-project-build.md)
    1. [Побудова за допомогою IDE](/doc/sections/uk_UA/5-project-build/5-1-IDE-build.md)
    1. [Побудова проекту через командний рядок](/doc/sections/uk_UA/5-project-build/5-2-command-line-build.md)
    1. [Швидкі скрипти побудови](/doc/sections/uk_UA/5-project-build/5-37-quick-build-scripts.md)
    1. [Налаштування ідентичності бази даних анотацій](/doc/sections/uk_UA/5-project-build/5-36-configuring-the-annotations-db-identity.md)
    1. Вмикання тестів
        1. [Вмикання юніт-тестів](/doc/sections/uk_UA/5-project-build/testing/5-3-1-enabling-unit-testing.md)
        1. [Запобігання використання GTest з ОС](/doc/sections/uk_UA/5-project-build/testing/5-3-2-disabling-system-GTest-probe.md)
    1. [Побудова документації](/doc/sections/uk_UA/5-project-build/documentation/5-4-documentation-build.md)
    1. [Вмикання підтримки встановлення документації](/doc/sections/uk_UA/5-project-build/documentation/5-5-configuring-the-documentation-install-support.md)
    1. [Налаштування складових імені встановлюваної бібліотеки](/doc/sections/uk_UA/5-project-build/compression/5-23-customizing-library-name-segments.md)
    1. Якість коду та санітайзери
        1. [Вмикання підтримки форматування коду](/doc/sections/uk_UA/5-project-build/code-quality/5-6-enabling-and-performing-code-formatting-target.md)
        1. [Вмикання підтримки цілі статичного аналізатора коду cppcheck](/doc/sections/uk_UA/5-project-build/code-quality/5-7-enabling-the-static-code-analyzer-target-with-cppcheck.md)
        1. [Вмикання підтримки статичного аналізатора коду clang-tidy](/doc/sections/uk_UA/5-project-build/code-quality/5-8-enabling-static-code-analyzer-with-clang-tidy.md)
        1. [Вмикання цілі динамічної перевірки пам'яті за допомогою valgrind](/doc/sections/uk_UA/5-project-build/code-quality/5-9-enabling-the-dynamic-memory-check-target-with-valgrind.md)
        1. [Вмикання санітайзерів](/doc/sections/uk_UA/5-project-build/code-quality/5-13-enabling-sanitizers.md)
    1. Контейнери та CI
        1. [Вмикання побудови і запуску у docker-контейнері](/doc/sections/uk_UA/5-project-build/containers-ci/5-12-enabling-the-docker-container-build-and-run.md)
        1. [Вмикання конвеєра Jenkins всередині Docker-контейнера](/doc/sections/uk_UA/5-project-build/containers-ci/5-17-enabling-Jenkins-pipeline-inside-Docker-container.md)
    1. Профайлери
        1. [Вмикання аналізу профайлером gprof](/doc/sections/uk_UA/5-project-build/profilers/5-15-enabling-gprof-profiler-analysis.md)
        1. [Вмикання аналізу профайлером callgrind від valgrind](/doc/sections/uk_UA/5-project-build/profilers/5-16-enabling-valgrinds-callgrind-profiler-analysis.md)
    1. Пакувальники
        1. [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерування пакунків flatpak](/doc/sections/uk_UA/5-project-build/packagers/5-11-enabling-the-flatpak-package-generation-support.md)
        1. [Вмикання пакетного менеджера snap](/doc/sections/uk_UA/5-project-build/packagers/5-19-enabling-the-snap-package-manager.md)
        1. [Вмикання підтримки генерування пакунків FreeBSD pkg з cpack](/doc/sections/uk_UA/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерування WIX MSI-пакетів з cpack](/doc/sections/uk_UA/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерування RPM-пакунків з cpack](/doc/sections/uk_UA/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md)
    1. Бібліотеки
        1. [Вмикання інтеграції libcurl](/doc/sections/uk_UA/5-project-build/5-14-enabling-libcurl.md)
        1. [Вмикання інтеграції бібліотеки nlohmann JSON](/doc/sections/uk_UA/5-project-build/5-18-enabling-the-nlohmann-json-library.md)
    1. Безпека / Криптографія
        1. [Вмикання інтеграції OpenSSL](/doc/sections/uk_UA/5-project-build/security/5-35-enabling-the-openssl-library.md)
    1. Стиснення даних
        1. [Вмикання інтеграції zlib](/doc/sections/uk_UA/5-project-build/compression/5-23-enabling-the-zlib-library.md)
        1. [Вмикання інтеграції liblzma](/doc/sections/uk_UA/5-project-build/compression/5-34-enabling-the-liblzma-library.md)
    1. Зображення
        1. [Вмикання інтеграції libpng](/doc/sections/uk_UA/5-project-build/image-libraries/5-24-enabling-the-libpng-library.md)
        1. [Вмикання інтеграції libjpeg](/doc/sections/uk_UA/5-project-build/image-libraries/5-25-enabling-the-libjpeg-library.md)
        1. [Вмикання інтеграції libwebp](/doc/sections/uk_UA/5-project-build/image-libraries/5-26-enabling-the-libwebp-library.md)
        1. [Вмикання інтеграції lunasvg (SVG)](/doc/sections/uk_UA/5-project-build/image-libraries/5-27-enabling-the-lunasvg-library.md)
        1. [Вмикання інтеграції giflib (GIF)](/doc/sections/uk_UA/5-project-build/image-libraries/5-28-enabling-the-giflib-library.md)
        1. [Вмикання інтеграції libtiff (TIFF)](/doc/sections/uk_UA/5-project-build/image-libraries/5-29-enabling-the-libtiff-library.md)
        1. [Вмикання інтеграції OpenEXR (EXR / HDR)](/doc/sections/uk_UA/5-project-build/image-libraries/5-30-enabling-the-openexr-library.md)
        1. [Вмикання інтеграції OpenJPEG (JPEG 2000)](/doc/sections/uk_UA/5-project-build/image-libraries/5-31-enabling-the-openjpeg-library.md)
        1. [Вмикання інтеграції libavif (AVIF)](/doc/sections/uk_UA/5-project-build/image-libraries/5-32-enabling-the-libavif-library.md)
        1. [Вмикання інтеграції libheif (HEIF/HEIC)](/doc/sections/uk_UA/5-project-build/image-libraries/5-33-enabling-the-libheif-library.md)
1. [Запуск доступних виконуваних файлів](/doc/sections/uk_UA/6-run-the-executable/6-run-avaialble-executables.md)
    1. [Запуск виконуваного файлу](/doc/sections/uk_UA/6-run-the-executable/6-run-the-executable.md)
        1. [Запуск за допомогою IDE](/doc/sections/uk_UA/6-run-the-executable/6-1-IDE-run.md)
        1. Запуск через командний рядок
            1. [Пошук згенерованого виконуваного файлу](/doc/sections/uk_UA/6-run-the-executable/6-2-1-searching-for-the-generated-executable.md)
            1. [Запуск згенерованого виконуваного файлу](/doc/sections/uk_UA/6-run-the-executable/6-2-2-starting-the-generated-executable.md)
    1. Запуск тестів
        1. [Запуск тестів за допомогою ctest](/doc/sections/uk_UA/6-run-the-executable/6-3-1-run-tests-by-the-ctest.md)
        1. [Ручний запуск тестів](/doc/sections/uk_UA/6-run-the-executable/6-3-2-manual-tests-run.md)
1. [Встановлення](/doc/sections/uk_UA/7-installing/7-installing.md)
1. [Використання бібліотеки у власному проекті](/doc/sections/uk_UA/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md)
