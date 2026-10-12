## Стиснений формат проекту imgajz1

Файл imgajz1 (`*.imgajz1`, стиснений (**z**ipped) JSON-файл **im**a**g**es **a**nnotator першої версії) - це другий формат файлу проекту бібліотеки: звичайний JSON-файл проекту [формату JSON проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md), стиснений gzip ([RFC 1952](https://www.rfc-editor.org/rfc/rfc1952)) за допомогою `ZlibController` бібліотеки. До JSON нічого не додається і з нього нічого не вилучається: розпакований файл imgajz1 - це той самий текст, який зберігає JSON-файл того самого проекту, байт у байт.

Проект займає у файлі imgajz1 приблизно в 16 разів менше місця на диску, ніж у файлі JSON: проект з 20000 зображень і 60000 прямокутників займає 16,3 МБ як файл JSON і 0,99 МБ як файл imgajz1, записується за 0,78 секунди замість 0,64 (стиснення його 16 МБ JSON-тексту) і відкривається за 0,72 секунди замість 0,74 (release-збірка).

### Якого формату файл

Формат визначає розширення імені файлу у шляху - і для завантаження, і для запису:

| Шлях | Формат |
| --- | --- |
| `*.imgajz1` (будь-якого регістру літер: `*.IMGAJZ1` теж) | файл проекту imgajz1, стиснений gzip файл проекту JSON |
| будь-який інший (і `*.json`, і шляхи без розширення) | файл проекту JSON |

Тож проект, відкритий з файлу `.json` і записаний у файл `.imgajz1`, далі продовжується у форматі imgajz1 - `store_db()` без аргументу пише формат шляху останнього завантаження чи запису, - і навпаки. В API нічого не змінюється: `load_db()`, `store_db()`, `open_annotations_db()` і `IADataDriversLib::libcall()` приймають будь-який із цих шляхів.

```cpp
#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers012;

auto db = iadd::IADataDriversFacade::open_annotations_db("cars.json");

if (db == nullptr || !db->store_db("cars.imgajz1")) {
  return false;
}

// звідси база даних продовжується з файлом cars.imgajz1
return db->store_db();
```

### Стандартні інструменти

Файл imgajz1 - це звичайний gzip-файл JSON-тексту, тож стандартні інструменти перетворюють ці два файли один на одного, а будь-який інструмент JSON читає розпакований проект:

```
# файл проекту JSON з файлу imgajz1
gzip -dc cars.imgajz1 > cars.json

# і файл проекту imgajz1 з файлу JSON
gzip -c cars.json > cars.imgajz1

# імена класів прямокутників проекту
gzip -dc cars.imgajz1 | jq -r '.annotations[].annotations[].annotations[].name' | sort -u
```

### Читання і запис

Записувач створює JSON-текст проекту, з якого пишеться файл проекту JSON, - з відступом у два пробіли - і стискає його у файл зі стандартним рівнем zlib. Текст створюється ще до відкриття файлу, тож рядок, якого JSON не може втримати (ім'я файлу у невалідному UTF-8, наприклад), провалює запис, а файл проекту лишається таким, яким був.

Читач розпаковує файл і читає JSON-текст так, як читається файл проекту JSON, з тією самою перевіркою. Він відхиляє пошкоджені та обрізані дані gzip, разом із кінцевим блоком gzip з розміром і CRC даних, а файл без стиснення gzip - файл проекту JSON з іменем `.imgajz1` - читається як він є, так, як такі файли читає `ZlibController`.

### Реалізація

Формат - це [Imgajz1LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h), нащадок [LibraryImpl](/src/dbs/AnnotationsDBs/LibraryImpl.h), через якого `AnnotationsDirDB` виконує завантаження і запис шляхів `.imgajz1` (його `impl_of()`). `LibraryImpl` зберігає все від формату JSON - розбір, перевірку і перетворення проекту та створення його JSON-тексту - і читає та пише цей текст через свої захищені віртуальні `read_text()` і `write_text()`, два методи, які перевизначає нащадок: вони розпаковують і стискають текст за допомогою `uncompress_from_file()` і `compress_to_file()` контролера `ZlibController` з [src/zlib](/src/zlib) - компонента контролера zlib гілки `appZlib` шаблону [cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template) в адаптації, яку зробив з нього [darknetxx](https://github.com/yuriysydor1991/darknetxx), - що робить [zlib](/doc/sections/uk_UA/5-project-build/compression/5-23-enabling-the-zlib-library.md) обов'язковою залежністю бібліотеки.
