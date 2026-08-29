Наступна діаграма описує структуру головного компоненту `lib`:

![компонент lib](/doc/diagrams/images/lib-class-structure.svg)

Діаграма покриває увесь шлях від поверхні публічного API аж до реалізації: інтерфейси фасаду з директорії [src/lib/facade/public](/src/lib/facade/public) (`IADataDriversLib`, `IAnnotationsDB` із його базовим `IImagesPathsDBProvider`, `IRecordsFactory`, `IDBMerger`, записи даних `ImageRecord` і `ImageRecordRect` та `IADataDriversContext`, який передається між ними), вхідні двері `IADataDriversFacade` разом із парою реалізації `LibMain` і `LibFactory` за ними, саму базу даних анотацій `AnnotationsDirDB`, драйвер файлу проекту `LibraryImpl`, через який вона читає і пише, JSON-перетворювачі, яким той драйвер делегує (де)серіалізацію, `DefaultDBMerger` і `RecordsFactory`.

Вихідні коди діаграм зберігаються у директорії [doc/diagrams/plantuml](/doc/diagrams/plantuml).
