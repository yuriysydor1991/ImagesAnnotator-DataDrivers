## Публічні інтерфейсні файли бібліотеки

Директорія [src/lib/facade/public](/src/lib/facade/public) містить публічні заголовкові підключаємі файли, придатні до встановлення. Вони визначають усю поверхню API бібліотеки - ніщо інше з проекту не є досяжним ззовні - і кожне оголошення всередині них належить до простору імен `ImagesAnnotatorDataDrivers012`.

| Заголовковий файл | Що оголошує |
|---|---|
| [IADataDriversFacade.h](/src/lib/facade/public/IADataDriversFacade.h) | Статичні фабричні вхідні двері: `create_library_context`, `create_default_lib`, `create_library`, `create_annotations_db` (двічі), `open_annotations_db`, `create_records_factory`, `create_db_merger`, а також повідомлювачі `library_version`, `default_db_root_app_name` і `default_db_app_version` |
| [IADataDriversContext.h](/src/lib/facade/public/IADataDriversContext.h) | Дані, що переносяться у виклик бібліотеки і з нього: вхідні `db_path`, `app_name`, `app_version`, `records_factory` і `merger` та результуючий вихід `db` |
| [IADataDriversLib.h](/src/lib/facade/public/IADataDriversLib.h) | Інтерфейс вхідної точки одного пострілу `libcall(ctx)` |
| [IAnnotationsDB.h](/src/lib/facade/public/IAnnotationsDB.h) | Центральна абстракція: сторона читання з її бази плюс `load_db`, `store_db` (двічі), `add_images_db`, `set_app_info`, `get_app_name` і `get_app_version` |
| [IImagesPathsDBProvider.h](/src/lib/facade/public/IImagesPathsDBProvider.h) | Сторона читання і запитів: `get_images_db`, `get_db_path`, `get_available_annotations`, `changed` і `delete_image_record` |
| [IRecordsFactory.h](/src/lib/facade/public/IRecordsFactory.h) | Політика створення записів: `create_image_record` і два перевантаження `create_image_rect_record` |
| [IDBMerger.h](/src/lib/facade/public/IDBMerger.h) | Політика злиття: єдиний метод `merge(dst, append)` |
| [ImageRecord.h](/src/lib/facade/public/ImageRecord.h) | Одне анотоване зображення: `abs_dir_path`, `path`, `tmppath`, `rects`, `current_rect`, `iwidth`, `iheight` і `imageScale` |
| [ImageRecordRect.h](/src/lib/facade/public/ImageRecordRect.h) | Один іменований прямокутник: `name`, `x`, `y`, `width` і `height` |
| [IRecord.h](/src/lib/facade/public/IRecord.h), [IProvider.h](/src/lib/facade/public/IProvider.h) | Порожні базові класи-мітки двох ієрархій вище |

Вміст директорії [src/lib/facade/public](/src/lib/facade/public) встановлюється у піддиректорію `include/<назва бібліотеки>` обраного кореня інсталяції, що з поточною назвою проекту і мажорною версією означає `include/ImagesAnnotatorDataDrivers-0.12/` - див. [Зміна назви проекту і назви бібліотеки](/doc/sections/uk_UA/4-project-structure/4-2-changing-the-project-and-executable-name.md) і [Налаштування сегментів назви встановлюваної бібліотеки](/doc/sections/uk_UA/5-project-build/compression/5-23-customizing-library-name-segments.md).

### Обидві форми включення працюють

Ціль бібліотеки експортує як звичайний корінь включень, так і його версійовану піддиректорію, тож встановлений споживач може написати будь-що з цього:

```
#include <ImagesAnnotatorDataDrivers-0.12/IADataDriversFacade.h>   // рекомендовано
#include <IADataDriversFacade.h>
```

Надавай перевагу формі з префіксом. Назви заголовкових файлів на подобі `ImageRecord.h` чи `IRecord.h` є достатньо загальними, щоб зіткнутись у завантаженому шляху включень, а префікс також робить очевидним, з якою версією бібліотеки розмовляє одиниця трансляції. Заголовкові файли включають один одного за їхніми простими назвами, що продовжує працювати в обох випадках, бо версійована піддиректорія експортується як окрема директорія включень.

### Простір імен несе версію

Публічний простір імен є `ImagesAnnotatorDataDrivers012` - мажорна і мінорна версії бібліотеки навмисно вштамповані у його назву, щоб дві версії бібліотеки могли бути присутніми в одній одиниці трансляції без зіткнення їхніх типів. Коду споживача рекомендується створити для нього псевдонім:

```
namespace iadd = ImagesAnnotatorDataDrivers012;
```

### CMake-пакунок

Окрім успішного встановлення двійкового файлу бібліотеки і заголовкових файлів, у систему встановлюється відповідний CMake-пакунок для забезпечення доступності бібліотеки для усіх зовнішніх зацікавлених проектів: `find_package(ImagesAnnotatorDataDrivers-0.12 REQUIRED)` приносить імпортовану ціль `ImagesAnnotatorDataDrivers-0.12::ImagesAnnotatorDataDrivers-0.12`, яка несе обидві згадані вище директорії включень. Переглянь CMake-файли [src/lib/cmake/lib-cmake-module-gen.cmake](/src/lib/cmake/lib-cmake-module-gen.cmake) і [src/lib/cmake/TemplateLibraryConfig.cmake.in](/src/lib/cmake/TemplateLibraryConfig.cmake.in) щодо подробиць генерації CMake-пакунку бібліотеки, а розділ [Використання бібліотеки у власному проекті](/doc/sections/uk_UA/8-using-the-library-in-your-project/8-using-the-library-in-your-project.md) - щодо повного прикладу споживача.

Зверни увагу, що ані залежність nlohmann json, ані libcurl не з'являється у жодному з цих заголовкових файлів: обидві злінковані як `PRIVATE`, тож проект-споживач не успадковує жодної з них і не потребує жодної з них у своєму шляху включень.
