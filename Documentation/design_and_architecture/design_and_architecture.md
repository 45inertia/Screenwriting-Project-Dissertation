# Design & Architecture

The design and architecture of the project are extremely important to the developoment phase. Below
are diagrams and research that has been conducted to form the design and architecture stage of
development.

## Open Screenplay Format
Before Designing the application we need to make sure it is compatible with the [open screenplay
format](https://github.com/OpenScreenplayFormat/osf-sdk/blob/master/v4/OSF-4.0.xml). From the
example open screenplay format xml file we can determine how the data should be handled before
it is passed into our "manager".

## The layers
For this project there are multiple layers of functionality that can be grouped using the MVVM 
model

### View
This contains the `MainWindow`, `ScriptEditor` and `SceneNavigator` classes. This layer is
responsible for the Qt widgets, user input and the display.

### ViewModel
This layer contains the `ScriptViewModel` class and controls the formatting rules, element cycling,
autocomplete, and file I/O delegation.

### Model
The models are `Script`, `Scene`, and `ScriptElement`. This is responsible for the pure data and
should have no Qt dependencies.

### Services
This layer contains `ScriptManager`, `OSFSerializer`, `PdfExporter`, `CharacterRegister` and is
responsible for the persistence, export and logic.

The **Services** layer has been added to the project as features such as the serializer and pdf
exporting do not fit into the ViewModel layer.

Therefore, the way that the ownership chain flows is that the `MainWindow` will create a 
`ScriptViewModel` object that in turn creates a `ScriptManager` object and both are kept on the
heap. Qt has its own parent-child ownership hierarchy so `ScriptViewModel` will be a QObject. The
benefit of having it done this way is that Qt can take care of any potential memory leaks and delete
child objects when the parent object is deleted. `ScriptManager` can be kept pure C++ and use smart
pointers

## Class Diagram
````mermaid
classDiagram

    class MainWindow {
        -ScriptEditor editor
        -SceneNavigator navigator
        -ScriptViewModel viewModel
        +setupUI() void
        +connectSignals() void
        +onNewFile() void
        +onOpenFile() void
        +onSaveFile() void
        +onExportPdf() void
    }

    class ScriptEditor {
        -ScriptViewModel viewModel
        +keyPressEvent(event) void
        +applyFormatting(type) void
        +refreshDisplay() void
        +onElementTypeChanged(type) void
    }

    class SceneNavigator {
        -ScriptViewModel viewModel
        +populateScenes(scenes) void
        +onSceneClicked(index) void
    }

    class ScriptViewModel {
        -ScriptManager scriptManager
        -CharacterRegister charRegister
        -ElementType currentType
        +getCurrentType() ElementType
        +cycleElementType() void
        +getSuggestions(prefix) List
        +loadScript(path) void
        +saveScript(path) void
        +exportPdf(path) void
        +getScenes() List
        +addElement(text, type) void
    }

    class ScriptManager {
        -Script script
        -OsfSerializer serializer
        +getScript() Script
        +loadFromFile(path) bool
        +saveToFile(path) bool
        +addScene(scene) void
        +addElement(element, sceneIndex) void
    }

    class OsfSerializer {
        +serialize(script) QByteArray
        +deserialize(xml) Script
        -parseScene(xml) Scene
        -parseElement(xml) ScriptElement
        -writeScene(xml, scene) void
        -writeElement(xml, element) void
    }

    class PdfExporter {
        +exportToPdf(script, path) bool
        -applyPageLayout(writer) void
        -renderElement(painter, element, yPos) void
        -computeFormatting(type) ElementFormatting
    }

    class CharacterRegister {
        -names QSet
        +buildFromScript(script) void
        +registerCharacter(name) void
        +getSuggestions(prefix) List
    }

    class Script {
        -QString title
        -QString author
        -scenes List
        +getTitle() QString
        +getAuthor() QString
        +getScenes() List
        +addScene(scene) void
        +sceneCount() int
    }

    class Scene {
        -int sceneNumber
        -elements List
        +getSceneNumber() int
        +getElements() List
        +addElement(element) void
        +getHeading() QString
    }

    class ScriptElement {
        -int id
        -ElementType type
        -QString text
        +getId() int
        +getType() ElementType
        +getText() QString
        +setText(text) void
    }

    class ElementType {
        <<enumeration>>
        SCENE_HEADING
        ACTION
        CHARACTER
        DIALOGUE
        PARENTHETICAL
        TRANSITION
        SHOT
    }

    class ElementFormatting {
        +int leftMarginTwips
        +int rightMarginTwips
        +bool allCaps
        +QString fontFamily
        +int fontSize
        +forType(type) ElementFormatting
    }

    MainWindow *-- ScriptEditor
    MainWindow *-- SceneNavigator
    MainWindow --> ScriptViewModel
    ScriptEditor --> ScriptViewModel
    SceneNavigator --> ScriptViewModel
    ScriptViewModel --> ScriptManager
    ScriptViewModel --> CharacterRegister
    ScriptManager *-- Script
    ScriptManager --> OsfSerializer
    Script "1" *-- "0..*" Scene
    Scene "1" *-- "0..*" ScriptElement
    ScriptElement --> ElementType
    ElementFormatting --> ElementType
    PdfExporter --> Script
    PdfExporter --> ElementFormatting
    CharacterRegister --> Script
````

**Notes**

- `OsfSerializer`
  - It is important to understand the the parser never holds the whole file in memory, it's like
    reading a book one line at a time.
  - `QByteArray` is Qt's container for raw bytes. This will be used as it ensures the script text
    will survive serialization without being corrupted. OSF files are in standard UTF-8. This is 
    what we get when you write a `QByteArray` produced by `QXmlStreamWriter`.



## Element Formatting Reference
These are the industry standards for element formatting:

| Element Type | Left Margin | Right Margin | Capitalization | Notes |
|---|---|---|---|---|
| `SCENE_HEADING` | 1.5 in | 1.0 in | ALL CAPS | e.g. `INT. OFFICE - DAY` |
| `ACTION` | 1.5 in | 1.0 in | Normal | Block description |
| `CHARACTER` | 3.7 in | 1.0 in | ALL CAPS | Centred above dialogue |
| `DIALOGUE` | 2.5 in | 2.5 in | Normal | Under character name |
| `PARENTHETICAL` | 3.1 in | 2.5 in | Normal | `(beat)` style |
| `TRANSITION` | 4.0 in | 1.0 in | ALL CAPS | e.g. `CUT TO:` |
| `SHOT` | 1.5 in | 1.0 in | ALL CAPS | Sub-heading |


## Sequence Diagram

## Data Flow / State Diagram

## Technology Stack
- C++ standard library
- Smart pointers
- Qt XML
- Qt Signals / Slots
- CMake



## Tools
- Lucidchart
- Mermaid.js
