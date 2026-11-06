## Формат проекту imgajz2 із сегментами

Файл imgajz2 (`*.imgajz2`, стиснений (**z**ipped) JSON-файл **im**a**g**es **a**nnotator другої версії) - це третій формат файлу проекту бібліотеки і єдиний, що зберігає сегменти - анотації-багатокутники `ImageRecordSegment`: стиснений gzip JSON-файл проекту, так само як [файл imgajz1](/doc/sections/uk_UA/4-project-structure/4-11-the-imgajz1-compressed-project-format.md), який до того ж зберігає багатокутник кожного сегмента під ключем `"segment"` поруч із його `"rect"` і оголошує свій формат у корені. Решта його документа - це документ [формату JSON проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md).

### Сегменти в документі

```
кореневий об'єкт
  "<app name>": { "version": "<app version>" }   елемент ідентичності
  "annotations": [ ... ]                         вихідні теки формату JSON
  "format": { "name": "imgajz2",                 оголошення формату
              "types": [ "rect", "segment" ] }

масив "annotations" запису зображення
  { "name": "dog",                               прямокутник
    "rect": { "x": 50, "y": 20, "width": 100, "height": 40 } },
  { "name": "cat",                               сегмент: його прямокутник і його багатокутник
    "rect": { "x": 10, "y": 20, "width": 20, "height": 40 },
    "segment": [ [0.0, 0.0], [1.0, 0.0], [0.5, 1.0] ] }
```

Значення `"segment"` - це масив точок `[x, y]` багатокутника у просторі 0..1 прямокутника `"rect"`: x - частка ширини прямокутника від його лівого краю, y - частка його висоти від верхнього. Тож точка `[0.5, 1.0]` сегмента `cat` - це піксель `(20, 60)` зображення, дві інші - його пікселі `(10, 20)` і `(30, 20)`, прямокутник є обмежувальною рамкою багатокутника, а прямокутник, переміщений чи змінений у розмірі застосунком, переносить багатокутник разом із собою. Анотація без ключа `"segment"` - це прямокутник, так, як його зберігає файл проекту JSON.

Оголошення `"format"` повідомляє іншому застосунку, що файл може містити сегменти. Його несе кожен файл imgajz2, який пише бібліотека, і файл без жодного сегмента теж, тоді як читачеві бібліотеки воно не потрібне: він читає і вміст проектів imgajz1 та JSON - проект прямокутників.

### Який формат зберігає сегменти

Формат визначає розширення імені файлу у шляху - і для завантаження, і для запису:

| Шлях | Формат | Анотації, які він зберігає |
| --- | --- | --- |
| `*.imgajz2` (будь-якого регістру літер: `*.IMGAJZ2` теж) | файл проекту imgajz2 | прямокутники і сегменти |
| `*.imgajz1` (будь-якого регістру літер) | файл проекту imgajz1 | лише прямокутники |
| будь-який інший | файл проекту JSON | лише прямокутники |

Тож запис бази даних, що містить сегмент, за шляхом `.json` чи `.imgajz1` зазнає невдачі - `store_db()` повертає `false`, записує помилку в журнал і лишає файл таким, яким він був, - замість того щоб записати сам лише прямокутник сегмента і втратити його багатокутник: проект із сегментами продовжується лише у файлах imgajz2. Читачі JSON і imgajz1, навпаки, читають анотацію з ключем `"segment"` - документа imgajz2 під їхнім іменем - як прямокутник її `"rect"`, ігноруючи цей ключ.

```cpp
#include <ImagesAnnotatorDataDrivers-0.15/IADataDriversFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers015;

auto db = iadd::IADataDriversFacade::open_annotations_db("cars.imgajz1");

if (db == nullptr || db->get_images_db().empty()) {
  return false;
}

auto ir = db->get_images_db().front();
auto segment = iadd::IADataDriversFacade::create_records_factory()
                   ->create_image_segment_record(ir);

// багатокутник у пікселях зображення, обмежувальною рамкою якого стає
// прямокутник
segment->name = "cat";
segment->add_point(10.0, 20.0);
segment->add_point(30.0, 20.0);
segment->add_point(20.0, 60.0);

ir->rects.emplace_back(segment);

// файл cars.imgajz1 не зберігає сегментів, тож звідси база даних продовжується
// з файлом cars.imgajz2
return db->store_db("cars.imgajz2");
```

Бібліотека, старіша за цей формат, сприймає шлях `.imgajz2` як звичайний файл проекту JSON і не може розібрати його дані gzip, тож застосунок, побудований на її основі, відмовляється від файлу imgajz2, замість того щоб відкрити його сегменти як прямокутники і втратити багатокутники під час наступного запису.

### Стандартні інструменти

Файл imgajz2 - теж звичайний gzip-файл JSON-тексту, тож `gzip -dc` видає його документ для будь-якого інструмента JSON, - хоча для бібліотеки файл проекту JSON із того самого тексту - це проект лише прямокутників:

```
# оголошення формату файлу проекту imgajz2
gzip -dc cars.imgajz2 | jq '.format'

# імена класів сегментів проекту
gzip -dc cars.imgajz2 | jq -r '.annotations[].annotations[].annotations[] | select(has("segment")) | .name' | sort -u
```

### Читання і запис

Файл пишеться і читається так, як файл imgajz1, з двома відмінностями: документ, який створює записувач, зберігає точки `"segment"` кожного сегмента й оголошення `"format"`, а читач будує анотацію з ключем `"segment"` через `create_image_segment_record()` фабрики записів, а решту - через її `create_image_rect_record()`, тож власна фабрика будує і сегменти.

### Реалізація

Формат - це [Imgajz2LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz2LibraryImpl.h), нащадок [Imgajz1LibraryImpl](/src/dbs/AnnotationsDBs/Imgajz1LibraryImpl.h), через якого `AnnotationsDirDB` виконує завантаження і запис шляхів `.imgajz2` (його `impl_of()`) і який отримує ідентичність застосунку і фабрику записів того `LibraryImpl`, що його тримає база даних. Він зберігає стиснені gzip `read_text()` і `write_text()` свого базового класу і перевизначає два захищені віртуальні фабричні методи перетворювачів [LibraryImpl](/src/dbs/AnnotationsDBs/LibraryImpl.h): `create_writer()`, який створює [ImageRecordSegments2JSONConverter](/src/dbs/AnnotationsDBs/converters/ImageRecordSegments2JSONConverter.h) - нащадка `ImageRecords2JSONConverter`, що пише ключі `"segment"` і `"format"`, - і `create_rect_reader()`, який створює [ImageRecordSegmentJSON2RecordConverter](/src/dbs/AnnotationsDBs/converters/ImageRecordSegmentJSON2RecordConverter.h) - нащадка `ImageRecordRectJSON2RecordConverter`, що читає сегменти. `ImageRecords2JSONConverter`, яким пишуться файли JSON і imgajz1, кидає виняток `std::invalid_argument` для сегмента, що провалює їхній запис ще до відкриття файлу.
