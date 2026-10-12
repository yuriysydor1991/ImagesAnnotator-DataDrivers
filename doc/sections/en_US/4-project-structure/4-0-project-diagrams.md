Here is a diagram of the main `lib` component:

![lib component](/doc/diagrams/images/lib-class-structure.svg)

The diagram covers the whole way from the public API surface down to the implementation: the facade interfaces of the [src/lib/facade/public](/src/lib/facade/public) directory (`IADataDriversLib`, `IAnnotationsDB` with its `IImagesPathsDBProvider` base, `IRecordsFactory`, `IDBMerger`, the `ImageRecord` and `ImageRecordRect` data records and the `IADataDriversContext` carried between them), the `IADataDriversFacade` front door together with the `LibMain` and `LibFactory` implementation pair behind it, the `AnnotationsDirDB` annotations database itself, the `LibraryImpl` project file driver it reads and writes through together with its `Imgajz1LibraryImpl` descendant of the gzip compressed imgajz1 files and the `ZlibController` that one compresses them with, the JSON converters that driver delegates the (de)serialization to, the `DefaultDBMerger` and the `RecordsFactory`.

The image is drawn by hand after the PlantUML source at [doc/diagrams/plantuml/lib-class-structure.puml](/doc/diagrams/plantuml/lib-class-structure.puml), which stays the complete description of the component: laid out automatically, it comes out several thousand pixels wide. Edit the source whenever the class structure changes and bring the drawing along with it - neither is produced by the build. Rendering the source into a directory of its own shows what to compare the drawing against, without overwriting it:

```
# from the project root directory

plantuml -tsvg -o "$(pwd)/build/diagrams" doc/diagrams/plantuml/lib-class-structure.puml
```
