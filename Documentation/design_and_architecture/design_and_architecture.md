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

classDiagram
    direction TB

    %% ─── VIEW LAYER ──────────────────────────────────────────────────────────
    class MainWindow {
        -ScriptEditor* editor
        -SceneNavigator* navigator
        -ScriptViewModel* viewModel
        +MainWindow(QWidget* parent)
        +setupUI() void
        +connectSignals() void
        +onNewFile() void
        +onOpenFile() void
        +onSaveFile() void
        +onExportPdf() void
    }

    class ScriptEditor {
        -ScriptViewModel* viewModel
        +ScriptEditor(QWidget* parent)
        +keyPressEvent(QKeyEvent* event) void
        +applyFormatting(ElementType type) void
        +refreshDisplay() void
        +onElementTypeChanged(ElementType type) slot
    }

    class SceneNavigator {
        -ScriptViewModel* viewModel
        +SceneNavigator(QWidget* parent)
        +populateScenes(QList~Scene*~ scenes) void
        +onSceneClicked(int index) slot
    }

    %% ─── VIEWMODEL LAYER ─────────────────────────────────────────────────────
    class ScriptViewModel {
        -ScriptManager* scriptManager
        -CharacterRegister* charRegister
        -ElementType currentType
        +ScriptViewModel()
        +getCurrentType() ElementType
        +cycleElementType() void
        +getSuggestions(QString prefix) QList~QString~
        +loadScript(QString path) void
        +saveScript(QString path) void
        +exportPdf(QString path) void
        +getScenes() QList~Scene*~
        +addElement(QString text, ElementType type) void
        +elementTypeChanged(ElementType) signal
        +scriptLoaded() signal
        +autoCompleteReady(QList~QString~) signal
    }

    %% ─── SERVICE LAYER ───────────────────────────────────────────────────────
    class ScriptManager {
        -unique_ptr~Script~ script
        -OsfSerializer* serializer
        +ScriptManager()
        +getScript() Script*
        +loadFromFile(QString path) bool
        +saveToFile(QString path) bool
        +addScene(Scene* scene) void
        +addElement(ScriptElement* element, int sceneIndex) void
    }

    class OsfSerializer {
        +serialize(Script* script) QByteArray
        +deserialize(QByteArray xml) unique_ptr~Script~
        -parseScene(QXmlStreamReader& xml) unique_ptr~Scene~
        -parseElement(QXmlStreamReader& xml) unique_ptr~ScriptElement~
        -writeScene(QXmlStreamWriter& xml, Scene* scene) void
        -writeElement(QXmlStreamWriter& xml, ScriptElement* el) void
    }

    class PdfExporter {
        +exportToPdf(Script* script, QString path) bool
        -applyPageLayout(QPdfWriter& writer) void
        -renderElement(QPainter& p, ScriptElement* el, int& yPos) void
        -computeFormatting(ElementType type) ElementFormatting
    }

    class CharacterRegister {
        -QSet~QString~ names
        +buildFromScript(Script* script) void
        +registerCharacter(QString name) void
        +getSuggestions(QString prefix) QList~QString~
    }

    %% ─── MODEL LAYER ─────────────────────────────────────────────────────────
    class Script {
        -QString title
        -QString author
        -QList~unique_ptr~Scene~~ scenes
        +Script(QString title, QString author)
        +getTitle() QString
        +getAuthor() QString
        +getScenes() QList~Scene*~
        +addScene(unique_ptr~Scene~ scene) void
        +sceneCount() int
    }

    class Scene {
        -int sceneNumber
        -QList~unique_ptr~ScriptElement~~ elements
        +Scene(int sceneNumber)
        +getSceneNumber() int
        +getElements() QList~ScriptElement*~
        +addElement(unique_ptr~ScriptElement~ el) void
        +getHeading() QString
    }

    class ScriptElement {
        -int id
        -ElementType type
        -QString text
        +ScriptElement(int id, ElementType type, QString text)
        +getId() int
        +getType() ElementType
        +getText() QString
        +setText(QString text) void
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
        +static ElementFormatting forType(ElementType t)
    }

    %% ─── RELATIONSHIPS ───────────────────────────────────────────────────────
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