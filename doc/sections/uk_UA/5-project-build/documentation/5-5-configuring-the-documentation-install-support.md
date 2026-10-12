## Вмикання підтримки встановлення документації

Наявна можливість підтримки встановлення документації за допомогою встановлення змінних `ENABLE_DOC_DOXYGEN` і `DOXYGEN_DO_INSTALL` у значення `ON` під час конфігурування проекту за допомогою команди:

```
# всередині кореневої директорії проекту

cmake -B build -S . -DENABLE_DOC_DOXYGEN=ON -DDOXYGEN_DO_INSTALL=ON
```

Змінна `DOXYGEN_OUT_HTML_NAME` сконфігурує назву для директорії HTML документації (передається до файлу `Doxyfile`). Її типовим значенням є `${PROJECT_LIBRARY_NAME}-html`, що для незміненого проекту означає `ImagesAnnotatorDataDrivers-0.13-html`.

З увімкненими обома змінними згенерована директорія `doc/ImagesAnnotatorDataDrivers-0.13-html` встановлюється цілком у піддиректорію `share/docs` префікса встановлення, під компонентом встановлення `Documentation`:

```
# всередині кореневої директорії проекту

cmake --build build --target Doxygen-doc
cmake --install build --prefix /usr/local

# документація тоді потрапляє у
# /usr/local/share/docs/ImagesAnnotatorDataDrivers-0.13-html/index.html
```

Зауваж, що документація має бути згенерована ціллю `Doxygen-doc` *перед* виконанням команди встановлення - правило встановлення лише копіює вже наявну директорію.
