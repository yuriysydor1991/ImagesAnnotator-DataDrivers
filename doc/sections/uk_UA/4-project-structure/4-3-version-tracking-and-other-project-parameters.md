## Версіювання і інші параметри проекту

Файли `CMakeLists.txt` проекту відслідковують поточний хеш коміту git-репозиторію проекту, назву бібліотеки, дату конфігурації і версію проекту, і передають їх у файл кодової конфігурації проекту, що знаходиться за шляхом [src/project-global-decls.h.in](/src/project-global-decls.h.in). Файл `project-global-decls.h.in` компілюється у директорію побудови як `project-global-decls.h`, тому будь-який файл реалізації бібліотеки дотягується до тих значень простим `#include "project-global-decls.h"`.

Усе оголошене там належить до простору імен `project_decls` і є звичайним `const std::string`:

| Константа | Заповнюється з | Значення з поточними типовими налаштуваннями |
|---|---|---|
| `project_decls::PROJECT_NAME` | змінної кешу `PROJECT_LIBRARY_NAME` | `ImagesAnnotatorDataDrivers-0.14` |
| `project_decls::PROJECT_BUILD_VERSION` | `PROJECT_VERSION` кореневого виклику `project()` | `0.14.0` |
| `project_decls::PROJECT_BUILD_COMMIT` | хешу git-коміту, на якому виконувалась конфігурація | напр. `966341c...` |
| `project_decls::PROJECT_CONFIGURE_DATE` | мітки часу конфігурації | напр. `2026-08-01 12:00:00` |
| `project_decls::ANNOTATIONS_DB_ROOT_APP_NAME` | змінної кешу `ANNOTATIONS_DB_ROOT_APP_NAME` | `ImagesAnnotator` |
| `project_decls::ANNOTATIONS_DB_APP_VERSION` | змінної кешу `ANNOTATIONS_DB_APP_VERSION` | `0.14.0` (версія проекту) |

Хеш git-коміту отримується модулем [cmake/enablers/template-project-git-enabler.cmake](/cmake/enablers/template-project-git-enabler.cmake), а мітка часу конфігурації разом із двома значеннями бази даних анотацій оголошуються у [cmake/template-project-misc-variables-declare.cmake](/cmake/template-project-misc-variables-declare.cmake).

Сам заголовковий файл є деталлю реалізації і **не** встановлюється. Значення, якими споживач може законно цікавитись, повторно експортуються через публічний фасад: `IADataDriversFacade::library_version()` повертає `PROJECT_BUILD_VERSION`, `IADataDriversFacade::default_db_root_app_name()` повертає `ANNOTATIONS_DB_ROOT_APP_NAME`, а `IADataDriversFacade::default_db_app_version()` повертає `ANNOTATIONS_DB_APP_VERSION`.

### Два параметри бази даних анотацій

Два останні рядки таблиці заслуговують на слово пояснення, бо вони взагалі не про бібліотеку.

Файл проекту анотацій є JSON-документом, укоріненим в елементі, названому на честь **програми**, яка його виробила. Файли, записані програмою [ImagesAnnotator](https://github.com/yuriysydor1991/ImagesAnnotator.git), несуть кореневий елемент `ImagesAnnotator`, і читач шукає саме той елемент. Якби бібліотека штампувала там власну назву, вона виробляла б файли, які програма більше не здатна прочитати, і відмовлялася б завантажувати наявні.

Саме тому ця назва є значенням, а не константою:

| CMake-змінна кешу | Типове значення | Значення |
|---|---|---|
| `ANNOTATIONS_DB_ROOT_APP_NAME` | `ImagesAnnotator` | Назва кореневого JSON-елемента файлів проектів: назва, що записується у вироблені файли, і та, що очікується всередині прочитаних |
| `ANNOTATIONS_DB_APP_VERSION` | версія проекту, `0.14.0` | Рядок версії, що зберігається під тим кореневим елементом і порівнюється з тим, який знайдено всередині прочитаного файлу |

Обидві є лише типовими значеннями часу компіляції. Кожен екземпляр може перевизначити їх під час виконання - або через поля `IADataDriversContext::app_name` і `IADataDriversContext::app_version` перед створенням бази даних, або опісля через `IAnnotationsDB::set_app_info(name, version)`. Розбіжність версій між файлом і сконфігурованим значенням не є фатальною - вона лише виробляє попередження у журналі - тоді як відсутній кореневий елемент призводить до провалу завантаження.

Див. підрозділ [Налаштування ідентичності бази даних анотацій](/doc/sections/uk_UA/5-project-build/5-36-configuring-the-annotations-db-identity.md) щодо рецептів часу конфігурації.
