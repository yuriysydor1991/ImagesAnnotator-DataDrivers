## API бази даних анотацій

Усе, чого дозволено торкатися проекту-споживачу, знаходиться у директорії [src/lib/facade/public](/src/lib/facade/public), і кожен без винятку з цих заголовкових файлів встановлюється у систему. Інших точок входу немає: класи реалізації (`AnnotationsDirDB`, `LibraryImpl`, `RecordsFactory`, `DefaultDBMerger` і решта вмісту [src/lib/libmain](/src/lib/libmain)) залишаються поза абстрактними інтерфейсами і ніколи не з'являються в одиниці трансляції проекту-споживача.

Ця секція - екскурсія цією поверхнею. Читати її варто з відкритими справжніми заголовковими файлами - вони містять doxygen-коментарі, з яких походить цей текст.

### Версійований простір імен

Кожен публічний тип знаходиться у просторі імен `ImagesAnnotatorDataDrivers011`. Суфікс `011` - це склеєні докупи головний і другорядний номери версії бібліотеки, і він там не випадково: дві різні версії бібліотеки можуть бути залучені до однієї й тієї ж одиниці трансляції без жодного конфлікту символів.

Виписувати цю назву повністю усюди було б болісно, тому достатньо один раз оголосити псевдонім:

```cpp
namespace iadd = ImagesAnnotatorDataDrivers011;
```

Підняття версії бібліотеки після цього залишається зміною в один рядок на вашому боці. Решта цієї секції користується псевдонімом `iadd`.

### Набір заголовкових файлів

| Заголовковий файл | Що він дає |
| --- | --- |
| [LibraryFacade.h](/src/lib/facade/public/LibraryFacade.h) | статичний фасад-фабрика - єдиний заголовковий файл, потрібний простому споживачу |
| [IAnnotationsDB.h](/src/lib/facade/public/IAnnotationsDB.h) | власне база даних анотацій: завантаження, збереження, злиття, ідентичність |
| [IImagesPathsDBProvider.h](/src/lib/facade/public/IImagesPathsDBProvider.h) | половина бази даних, що відповідає за читання і запити |
| [ImageRecord.h](/src/lib/facade/public/ImageRecord.h) | одне анотоване зображення |
| [ImageRecordRect.h](/src/lib/facade/public/ImageRecordRect.h) | один іменований прямокутник, намальований поверх зображення |
| [IRecordsFactory.h](/src/lib/facade/public/IRecordsFactory.h) | точка розширення десеріалізації |
| [IDBMerger.h](/src/lib/facade/public/IDBMerger.h) | точка розширення злиття |
| [LibraryContext.h](/src/lib/facade/public/LibraryContext.h) | тримач вхідних і вихідних параметрів |
| [ILib.h](/src/lib/facade/public/ILib.h) | одноразова точка входу `libcall(ctx)` |
| [IRecord.h](/src/lib/facade/public/IRecord.h), [IProvider.h](/src/lib/facade/public/IProvider.h) | порожні маркувальні базові класи, від яких походить кожен запис і кожен постачальник |

Кожен інтерфейсний заголовковий файл також оголошує поряд з класом псевдонім `...Ptr` - `iadd::IAnnotationsDBPtr`, `iadd::ImageRecordPtr`, `iadd::ImageRecordRectPtr`, `iadd::IRecordsFactoryPtr`, `iadd::IDBMergerPtr`, `iadd::LibraryContextPtr`, `iadd::ILibPtr` - і всі вони є спеціалізаціями `std::shared_ptr`. Усе, що бібліотека видає назовні, перебуває у спільному володінні; немає нічого, що треба було б звільняти через `delete`, і нічого, що треба було б повертати назад.

### `LibraryFacade` - парадний вхід

`iadd::LibraryFacade` - це клас, що складається виключно зі статичних методів. Він ніколи не створюється як об'єкт, не тримає жодного стану, і кожен його виклик будує цілком новий незалежний об'єкт.

| Метод | Що повертає |
| --- | --- |
| `create_library_context()` | `LibraryContextPtr`, попередньо заповнений значеннями за замовчуванням часу компіляції |
| `create_default_lib()` | реалізацію `ILib` за замовчуванням |
| `create_library(ctx)` | реалізацію `ILib`, відповідну наданому контексту |
| `create_annotations_db()` | порожню базу даних із ідентичністю за замовчуванням часу компіляції |
| `create_annotations_db(ctx)` | порожню базу даних, налаштовану наданим контекстом |
| `open_annotations_db(fpath)` | базу даних із вже завантаженим вказаним файлом проекту, або `nullptr` |
| `create_records_factory()` | `IRecordsFactory` бібліотеки за замовчуванням |
| `create_db_merger()` | `IDBMerger` бібліотеки за замовчуванням |
| `library_version()` | рядок версії того бінарного файлу бібліотеки, що використовується |
| `default_db_root_app_name()` | значення `ANNOTATIONS_DB_ROOT_APP_NAME` часу компіляції |
| `default_db_app_version()` | значення `ANNOTATIONS_DB_APP_VERSION` часу компіляції |

Останні три - це прості інформаційні виклики, зручні для вікна "про програму" або для рядка у журналі під час запуску. Пара значень ідентичності пояснюється у секції [налаштування ідентичності бази даних анотацій](/doc/sections/uk_UA/5-project-build/5-36-configuring-the-annotations-db-identity.md).

### Три шляхи всередину

#### 1. `open_annotations_db()` - звичайний випадок

Один рядок, один файл, і по всьому:

```cpp
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

auto db = iadd::LibraryFacade::open_annotations_db("project.json");

if (db == nullptr) {
  // файл був відсутній, нечитабельний, взагалі не JSON-документ
  // або не є дійсним проектом анотацій - див. розділ про JSON-формат
  return false;
}
```

Він створює базу даних за замовчуванням і одразу викликає для неї `load_db()`, повертаючи `nullptr` тоді, коли те завантаження зазнало невдачі. Побудована ним база даних несе ідентичність застосунку часу компіляції, а також власні фабрику записів і об'єднувач бібліотеки. Коли цього достатньо - а для читача, інспектора чи конвертера зазвичай саме так і є - можна зупинитися тут.

#### 2. `create_annotations_db(ctx)` - коли важать колаборанти

По варіант із контекстом слід тягнутися одразу, щойно одна з трьох речей, які він несе, має бути іншою: ідентичність застосунку, що записується у файл, фабрика записів, якою десеріалізація будує записи, або об'єднувач, яким `add_images_db()` виконує свою роботу.

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->app_name = "MyTool";
ctx->app_version = "2.0";

auto db = iadd::LibraryFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("project.json")) {
  return false;
}
```

Щойно створений контекст вже придатний до вжитку: його `app_name` і `app_version` заповнені значеннями за замовчуванням часу компіляції, а його `records_factory` і `merger` порожні, що означає "використовуй власні бібліотечні". Контекст `nullptr` теж приймається і трактується як порожній контекст за замовчуванням.

Варто зважати на одну асиметрію: `create_annotations_db()` створює **порожню** базу даних. Вона ніколи не заглядає у `ctx->db_path` - це поле належить шляху `libcall()`, описаному далі - тож виклик `load_db()` залишається за вами.

#### 3. `ILib::libcall()` - одноразовий шлях

`ILib` - це односкладовий інтерфейс, за яким ховаються варіанти реалізації бібліотеки. Заповнюємо контекст, передаємо його і отримуємо готову базу даних назад через той самий контекст:

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->db_path = "project.json";

auto lib = iadd::LibraryFacade::create_library(ctx);

if (lib == nullptr || !lib->libcall(ctx)) {
  return false;
}

auto db = ctx->db;  // це iadd::IAnnotationsDBPtr
```

`libcall()` будує базу даних з контексту точно так само, як це робить `create_annotations_db(ctx)`, а потім, **лише якщо `ctx->db_path` не порожній**, завантажує у неї вказаний файл. Порожній `db_path` не є помилкою: ви просто отримаєте налаштовану порожню базу даних у `ctx->db`.

Поля `LibraryContext` відповідно поділяються на вхідну і вихідну половини:

| Поле | Напрямок | Значення |
| --- | --- | --- |
| `db_path` | вхід | файл проекту, який має відкрити `libcall()`. Порожнє значення залишає базу даних порожньою |
| `app_name` | вхід | ім'я кореневого елемента JSON. За замовчуванням - значення часу компіляції |
| `app_version` | вхід | рядок версії, що зберігається під тим кореневим елементом |
| `records_factory` | вхід | необов'язкова власна `IRecordsFactory`. Порожнє значення означає бібліотечну за замовчуванням |
| `merger` | вхід | необов'язковий власний `IDBMerger`. Порожнє значення означає бібліотечний за замовчуванням |
| `db` | вихід | база даних, вироблена останнім викликом `libcall()` |

Цим шляхом варто скористатися тоді, коли ваша власна архітектура вже передає туди-сюди щось на кшталт контексту, або коли єдиний виклик, який або повністю вдається, або повністю провалюється, пасує найкраще. Коли ж потрібен більш дрібнозернистий контроль - краще йти через методи-фабрики фасаду.

### Поверхня `IAnnotationsDB`

`IAnnotationsDB` розширює призначений лише для читання `IImagesPathsDBProvider`. Цей поділ є навмисним: передайте коду, який лише *читає* записи, `iadd::IImagesPathsDBProviderPtr`, і він стане тривіально тестованим, а також тривіально живленим з альтернативного власного джерела у пам'яті.

Половина читання і запитів, з `IImagesPathsDBProvider`:

| Метод | Поведінка |
| --- | --- |
| `ImageRecordsSet& get_images_db()` | **змінюване посилання** на утримувані записи |
| `std::string get_db_path()` | шлях останнього завантаженого або збереженого файлу проекту, або порожній рядок |
| `AnnotationsList get_available_annotations()` | відсортований `std::set<std::string>` з кожним відмінним іменем прямокутника у всій базі даних |
| `bool changed()` | істина тоді, коли записи у пам'яті відрізняються від збережених |
| `bool delete_image_record(const std::string& irFullPath)` | стирає кожен запис, чий `get_full_path()` дорівнює аргументу |

`delete_image_record()` звітує про спробу, а не про кількість влучань: він повертає `true` навіть тоді, коли нічого не збіглося. Порівнюйте розмір набору до і після, коли треба знати напевно.

Половина завантаження і збереження, додана `IAnnotationsDB`:

| Метод | Поведінка |
| --- | --- |
| `bool load_db(const std::string& fpath)` | розбирає файл і **заміщує** записи у пам'яті тим, що він містить. Цей шлях стає призначенням збереження за замовчуванням |
| `bool store_db()` | записує записи назад за тим останнім використаним шляхом |
| `bool store_db(const std::string& fpath)` | записує записи за вказаним шляхом, який стає новим призначенням за замовчуванням |
| `void add_images_db(const ImageRecordsSet& andb)` | вливає інший набір записів через `IDBMerger` |
| `void set_app_info(const std::string& name, const std::string& version)` | перевизначає ідентичність, що записується у файли і очікується всередині них |
| `std::string get_app_name() const` | ім'я кореневого елемента JSON, що використовується наразі |
| `std::string get_app_version() const` | рядок версії, що використовується наразі |

Варто зауважити, що `load_db()` **заміщує**, а не додає. Завантаження другого файлу проекту поверх першого викидає перший геть; `add_images_db()` - ось той виклик, що поєднує два набори.

### `get_images_db()` видає змінюване посилання

Це той момент, про який варто сказати прямо. `get_images_db()` повертає `ImageRecordsSet&` - живе посилання всередину бази даних, а не копію:

```cpp
for (const auto& ir : db->get_images_db()) {
  for (const auto& rect : ir->rects) {
    rect->name = "cat";          // редагує базу даних на місці
  }
}

db->get_images_db().pop_back();  // і це також
```

Записи є `std::shared_ptr`-ами, тож редагування запису через копію вказівника редагує той самісінький запис. Це і є весь API редагування: сеттерів немає, ви змінюєте записи, а потім їх зберігаєте. Наслідки, про які варто пам'ятати:

- це посилання і кожен `ImageRecordPtr`, який ви з нього дістали, стають недійсними після наступного `load_db()`, який підмінює весь набір;
- `add_images_db()` може перевиділити вектор, тож не варто тримати ітератори через нього;
- бібліотека сама по собі не є потокобезпечною. Один примірник бази даних, керований з декількох потоків, потребує вашої власної синхронізації.

Потрібна натомість від'єднана копія, з якою можна погратися? `iadd::ImageRecord::duplicate(set)` глибоко копіює цілий набір, а `duplicate_shared()` робить те саме для окремого запису чи прямокутника.

### Типи-значення записів

`ImageRecord` - це одне анотоване зображення. Це простий тримач даних із жменькою зручностей:

| Член | Значення |
| --- | --- |
| `std::string abs_dir_path` | коренева директорія, з якої походить запис - або URL веб-сторінки |
| `std::string path` | шлях до зображення відносно `abs_dir_path` |
| `std::filesystem::path tmppath` | шлях локального кешу зображення, попередньо завантаженого з мережі. **Ніколи** не записується у файл проекту |
| `ImageRecordRectSet rects` | прямокутники анотацій |
| `ImageRecordRectPtr current_rect` | прямокутник, який наразі виділено шаром представлення. Теж не серіалізується |
| `int iwidth`, `int iheight` | власний розмір зображення |
| `double imageScale` | масштаб попереднього перегляду для цього зображення. `0` означає "користувач ще не обрав масштаб" |

Його методи: `get_full_path()` повертає `tmppath`, коли зображення було попередньо завантажене з мережі, і `abs_dir_path`, з'єднаний із `path`, в іншому випадку - саме за цим значенням `delete_image_record()` шукає збіг і саме за ним записи сортуються; `erase_current_rect()` викидає виділений прямокутник; `scaleStepIn()` / `scaleStepOut()` крокують `imageScale` на `defaultScaleStep` у межах `minScale` .. `maxScale`; `duplicate_shared()`, `duplicate()` і дві статичні перевантажені `equal()` виконують глибоке копіювання і порівняння за значеннями.

`ImageRecordRect` - це іменований прямокутник - `name`, `x`, `y`, `width`, `height` - плюс ті самі помічники `duplicate_shared()` і статичні `equal()`. Координати подані у власному піксельному просторі зображення, тож вони залишаються дійсними незалежно від того, який масштаб попереднього перегляду застосовує шар представлення.

Обидва типи можна конструювати напряму (`iadd::ImageRecord::create(...)` і конструктори `ImageRecordRect` є публічними), проте краще користуватися фабрикою - див. нижче - щоб власна фабрика залишалася за головного.

### Контракт `changed()` / `store_db()`

База даних тримає глибоку копію записів у тому вигляді, в якому вони були на момент останнього успішного завантаження чи збереження. `changed()` порівнює живий набір із тим знімком **за значеннями**, запис за записом і прямокутник за прямокутником:

- запис вважається рівним тоді, коли збігаються його `abs_dir_path`, `path`, `imageScale`, `iwidth`, `iheight` і вся його послідовність прямокутників;
- прямокутники мають збігатися за іменем, геометрією **і порядком**;
- `tmppath` і `current_rect` не є частиною порівняння. Це стан представлення, і він ніколи не серіалізується.

А це означає, що `changed()` чесний в обидва боки: скасуйте зміну вручну - і він знову звітуватиме `false`. Переставте місцями два прямокутники - і він звітує `true`, бо файл вийшов би іншим.

Успішний `store_db()` оновлює знімок, тож одразу після нього `changed()` повертає `false`. Невдалий знімка не торкається, і незбережені модифікації залишаються незбереженими. Тож ідіома така:

```cpp
if (db->changed() && !db->store_db()) {
  // запис провалився - дані все ще у пам'яті, не виходь мовчки
  return false;
}
```

Одна пастка: `store_db()` без аргументів пише за шляхом останнього завантаження чи збереження, а база даних, створена через `create_annotations_db()`, ніколи такого не мала. Виклик там зазнає невдачі. Дайте щойно збудованій базі даних явне призначення один раз:

```cpp
auto db = iadd::LibraryFacade::create_annotations_db();

db->add_images_db(my_records);

if (!db->store_db("/tmp/new-project.json")) {
  return false;
}

// тепер той шлях є типовим, і простий store_db() відтепер працює
```

### Точка розширення 1: `IRecordsFactory`

Кожен запис, який виробляє десеріалізація, будується через `IRecordsFactory`. Підставте власну - і JSON-конвертери почнуть видавати вам ваші власні збагачені підкласи, а це і є спосіб причепити до запису все, що потрібно вашому застосунку (завантажений pixbuf, ідентифікатор у базі даних, прапорець змінності), так, що бібліотека про це нічого й не знатиме.

Інтерфейс складається з трьох методів:

```cpp
virtual ImageRecordPtr create_image_record(const std::string& nirpath = "",
                                           const std::string& nabsPath = "") = 0;

virtual ImageRecordRectPtr create_image_rect_record(
    const ImageRecordPtr& ir = nullptr) = 0;

virtual ImageRecordRectPtr create_image_rect_record(const std::string& nname,
                                                    const int& nx, const int& ny,
                                                    const int& nwidth,
                                                    const int& nheight) = 0;
```

Перший приймає шлях відносно кореня і сам корінь. Другий створює **порожній** прямокутник з автоматично згенерованим іменем: із записом `nullptr` це константа `default_name_record` (`"rectangle_default"`), інакше це `default_numbered_name_record_prefix`, за яким іде поточна кількість прямокутників наданого запису, тож свіжий прямокутник для зображення, що вже має два, вийде як `rectangle_default_2`. Третій будує повністю визначений прямокутник, і саме його викликає JSON-десеріалізація.

Власна фабрика не мусить перереалізовувати геть усе - делегуйте бібліотечній фабриці за замовчуванням і торкайтеся лише того, що вас цікавить:

```cpp
#include <cctype>
#include <memory>
#include <string>

#include <ImagesAnnotatorDataDrivers-0.11/IRecordsFactory.h>
#include <ImagesAnnotatorDataDrivers-0.11/LibraryFacade.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

/// Будує рівно ті записи, що й типова фабрика бібліотеки, але зводить
/// назву класу кожної анотації до нижнього регістру.
class LowerCaseRecordsFactory : public iadd::IRecordsFactory
{
 public:
  LowerCaseRecordsFactory()
      : defaults{iadd::LibraryFacade::create_records_factory()}
  {
  }

  iadd::ImageRecordPtr create_image_record(const std::string& relPath,
                                           const std::string& absPath) override
  {
    return defaults->create_image_record(relPath, absPath);
  }

  iadd::ImageRecordRectPtr create_image_rect_record(
      const iadd::ImageRecordPtr& ir) override
  {
    return defaults->create_image_rect_record(ir);
  }

  iadd::ImageRecordRectPtr create_image_rect_record(const std::string& name,
                                                    const int& x, const int& y,
                                                    const int& width,
                                                    const int& height) override
  {
    return defaults->create_image_rect_record(lowered(name), x, y, width,
                                              height);
  }

 private:
  static std::string lowered(std::string s)
  {
    for (auto& c : s) {
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    return s;
  }

  iadd::IRecordsFactoryPtr defaults;
};
```

### Точка розширення 2: `IDBMerger`

`IDBMerger` - це один метод, `merge(ImageRecordsSet& dst, const ImageRecordsSet& append)`, і саме він визначає, що означає `add_images_db()`. Реалізація бібліотеки за замовчуванням додає лише ті записи, чий розв'язаний шлях ще не присутній у призначенні, а потім сортує призначення за повними шляхами записів. "Розв'язаний" - цікаве тут слово: запис, що вказує на веб-URL, перед порівнянням розв'язується через URL API `CURLU` бібліотеки libcurl, тож одне й те саме зображення, згадане і як голий відносний шлях відносно URL сторінки, і як його абсолютний URL, розпізнається як один запис.

Замінюйте його тоді, коли ваші правила інші - скажімо, ви збираєте кілька часткових проходів анотування тих самих зображень і хочете зберегти їх усі:

```cpp
#include <ImagesAnnotatorDataDrivers-0.11/IDBMerger.h>

namespace iadd = ImagesAnnotatorDataDrivers011;

/// Додає усе, включно з дублікатами, і зберігає заданий порядок.
class AppendAllMerger : public iadd::IDBMerger
{
 public:
  void merge(iadd::ImageRecordsSet& dst,
             const iadd::ImageRecordsSet& append) override
  {
    dst.reserve(dst.size() + append.size());

    for (const auto& ir : append) {
      dst.emplace_back(ir->duplicate_shared());
    }
  }
};
```

### Як їх під'єднати

Обидві точки розширення впроваджуються через контекст, і обидві є необов'язковими - залиште поле порожнім, і буде використано бібліотечне значення за замовчуванням:

```cpp
auto ctx = iadd::LibraryFacade::create_library_context();

ctx->records_factory = std::make_shared<LowerCaseRecordsFactory>();
ctx->merger = std::make_shared<AppendAllMerger>();

auto db = iadd::LibraryFacade::create_annotations_db(ctx);

if (db == nullptr || !db->load_db("project.json")) {
  return false;
}

// записи були побудовані твоєю фабрикою під час завантаження вище
db->add_images_db(another_set);  // а злиті твоїм злиттям тут
```

Той самий контекст іде до `ILib::libcall()`, якщо вам більше до вподоби одноразовий шлях. Варто ще раз зауважити, що `open_annotations_db()` не приймає контексту і тому завжди користується значеннями бібліотеки за замовчуванням - це скорочення, а не налаштовуваний маршрут.

### Куди рухатися далі

- Вигляд на диску того, що все це читає і пише: [формат JSON проекту анотацій](/doc/sections/uk_UA/4-project-structure/4-10-the-annotations-project-json-format.md).
- Ідентичність, яку несуть вироблені файли: [налаштування ідентичності бази даних анотацій](/doc/sections/uk_UA/5-project-build/5-36-configuring-the-annotations-db-identity.md).
- Побудова власної програми проти встановленого пакунка: [використання бібліотеки у власному проекті](/doc/sections/uk_UA/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md).
- Доказ того, що публічних заголовкових файлів самих по собі достатньо: компонентний тест `CTEST_AnnotationsDirDB` за шляхом [src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB](/src/dbs/AnnotationsDBs/tests/component/AnnotationsDirDB) лінкується зі справжньою спільною бібліотекою і дістається до неї виключно через ці заголовкові файли. Це гарне джерело подальших прикладів використання.
