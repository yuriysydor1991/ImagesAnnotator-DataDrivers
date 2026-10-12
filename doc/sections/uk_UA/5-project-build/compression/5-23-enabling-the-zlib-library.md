## Вмикання інтеграції zlib

[zlib](https://zlib.net/) є обов'язковою залежністю цієї бібліотеки: файли
проекту imgajz1, які бібліотека пише і читає, є стиснутими gzip
JSON-документами,
які стискає і розпаковує `ZlibController` з [src/zlib](/src/zlib). Цей
контролер - це компонент контролера zlib з
[cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template) (його
гілки `appZlib`) в адаптації, яку зробив з нього
[darknetxx](https://github.com/yuriysydor1991/darknetxx) для своїх файлів
dxxwjz: стиснення в пам'яті і у gzip-файли та потоки gzip-файлів, які читають
і пишуть файли частина за частиною. Тому CMake-опція `ENABLE_ZLIB` типово має
значення `ON`, і для звичайної побудови нічого вмикати не потрібно:

```
# в середині кореневої директорії проекту

cmake -S . -B build
```

Значення `ENABLE_ZLIB=OFF` зупиняє конфігурування з помилкою, оскільки без zlib
бібліотека не будується.

Інтеграція zlib розділена на два CMake-модулі:

- [cmake/enablers/compression/template-project-zlib-enabler.cmake](/cmake/enablers/compression/template-project-zlib-enabler.cmake)
  лише робить залежність доступною. Він виконується до додавання `src/`, тож
  виконувані файли модульних тестів з дерева джерельних кодів уже можуть самі
  лінкувати `ZLIB::ZLIB`.
- [cmake/enablers/ImagesAnnotator-zlib-linker.cmake](/cmake/enablers/ImagesAnnotator-zlib-linker.cmake)
  виконується після `src/` і лінкує zlib з ціллю бібліотеки, яка існує лише
  тоді. Лінкування `PRIVATE`: жоден публічний заголовок, який встановлюється,
  не відкриває типів zlib, тож залежність не потрапляє в експортований
  інтерфейс бібліотеки.

Спершу перевіряється наявність системної zlib за допомогою `find_package`. Коли
її не знайдено, джерельні коди завантажуються за допомогою `FetchContent` з
репозиторію і теґу зі змінних кешу `TEMPLATE_APP_ZLIB_GIT` і
`TEMPLATE_APP_ZLIB_GIT_TAG` (типово теґ `v1.3.1`), і статична бібліотека
`zlibstatic`, яку вони будують, лінкується під тим самим іменем `ZLIB::ZLIB`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DTEMPLATE_APP_ZLIB_GIT_TAG=v1.3.1
```

Встановлення системного пакунка розробки (наприклад, `zlib1g-dev` у
дистрибутивах на базі Debian) - швидший шлях, який залишає крок конфігурування
без доступу до мережі.

Сусідні бібліотеки [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git)
і [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git)
містять той самий контролер для файлів dxxwjz darknetxx, кожна у власному
просторі імен: ця бібліотека експортує всі свої символи, тож її контролер живе
у просторі імен `iadd0zlibi` замість простору імен `zlibi` шаблону.
