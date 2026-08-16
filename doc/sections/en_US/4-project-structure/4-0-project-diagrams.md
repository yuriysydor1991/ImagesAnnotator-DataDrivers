Here is a diagram of the main `lib` component:

![lib component](/doc/diagrams/images/lib-class-structure.svg)

The diagram covers the whole way from the public API surface down to the implementation: the facade interfaces of the [src/lib/facade/public](/src/lib/facade/public) directory (`ILib`, `IAnnotationsDB` with its `IImagesPathsDBProvider` base, `IRecordsFactory`, `IDBMerger`, the `ImageRecord` and `ImageRecordRect` data records and the `LibraryContext` carried between them), the `LibraryFacade` front door together with the `LibMain` and `LibFactory` implementation pair behind it, the `AnnotationsDirDB` annotations database itself, the `LibraryImpl` project file driver it reads and writes through, the JSON converters that driver delegates the (de)serialization to, the `DefaultDBMerger` and the `RecordsFactory`.

The diagram sources are kept in the [doc/diagrams/plantuml](/doc/diagrams/plantuml) directory.
