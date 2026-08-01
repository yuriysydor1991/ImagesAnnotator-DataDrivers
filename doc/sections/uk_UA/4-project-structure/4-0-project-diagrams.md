Наступна діаграма описує структуру головного компоненту `lib`:

![компонент lib](/doc/diagrams/images/lib-class-structure.svg)

Діаграма покриває увесь шлях від поверхні публічного API аж до реалізації: інтерфейси фасаду з директорії [src/lib/facade/public](/src/lib/facade/public) (`ILib`, `IAnnotationsDB` із його базовим `IImagesPathsDBProvider`, `IRecordsFactory`, `IDBMerger`, записи даних `ImageRecord` і `ImageRecordRect` та `LibraryContext`, який передається між ними), вхідні двері `LibraryFacade` разом із парою реалізації `LibMain` і `LibFactory` за ними, саму базу даних анотацій `AnnotationsDirDB`, JSON-перетворювачі, яким вона делегує (де)серіалізацію, `DefaultDBMerger` і `RecordsFactory`.

Вихідні коди діаграм зберігаються у директорії [doc/diagrams/plantuml](/doc/diagrams/plantuml).
