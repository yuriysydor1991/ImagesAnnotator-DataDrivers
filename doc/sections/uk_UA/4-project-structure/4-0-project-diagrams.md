Наступна діаграма описує структуру головного компоненту `lib`:

![компонент lib](/doc/diagrams/images/lib-class-structure.svg)

Діаграма покриває увесь шлях від поверхні публічного API аж до реалізації: інтерфейси фасаду з директорії [src/lib/facade/public](/src/lib/facade/public) (`IADataDriversLib`, `IAnnotationsDB` із його базовим `IImagesPathsDBProvider`, `IRecordsFactory`, `IDBMerger`, записи даних `ImageRecord` і `ImageRecordRect` та `IADataDriversContext`, який передається між ними), вхідні двері `IADataDriversFacade` разом із парою реалізації `LibMain` і `LibFactory` за ними, саму базу даних анотацій `AnnotationsDirDB`, драйвер файлу проекту `LibraryImpl`, через який вона читає і пише, разом із його нащадком `Imgajz1LibraryImpl` стиснених gzip файлів imgajz1 і `ZlibController`, яким той їх стискає, JSON-перетворювачі, яким той драйвер делегує (де)серіалізацію, `DefaultDBMerger` і `RecordsFactory`.

Зображення намальовано вручну за джерелом PlantUML [doc/diagrams/plantuml/lib-class-structure.puml](/doc/diagrams/plantuml/lib-class-structure.puml), яке лишається повним описом компонента: розташоване автоматично, воно виходить завширшки кілька тисяч пікселів. Редагуй джерело щоразу, коли змінюється структура класів, і змінюй малюнок разом із ним - жодне з них не створюється збіркою. Генерування джерела в окрему директорію показує, з чим порівнювати малюнок, не перезаписуючи його:

```
# з кореневої директорії проекту

plantuml -tsvg -o "$(pwd)/build/diagrams" doc/diagrams/plantuml/lib-class-structure.puml
```
