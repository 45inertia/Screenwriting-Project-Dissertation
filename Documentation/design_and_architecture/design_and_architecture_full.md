# Design & Architecture — Open Source Screenwriting Application

**Project:** Developing Open Source Screenwriting Software  
**Author:** Oliver Myers | COM629  
**Stack:** C++, Qt 6, Qt XML, CMake  
**Pattern:** MVVM (Model–View–ViewModel)

---

## 1. Overview

The application follows the **MVVM** pattern, separating the Qt UI layer from the
business logic and data models. A central `ScriptManager` owns the document model
and is responsible for all OSF XML serialisation. A `ScriptViewModel` mediates
every interaction between the view widgets and the model, preventing any direct
coupling between the two.

---

## 2. Architecture Layers

| Layer | Classes | Responsibility |
|---|---|---|
| **View** | `MainWindow`, `ScriptEditor`, `SceneNavigator` | Qt widgets, user input, display |
| **ViewModel** | `ScriptViewModel` | Formatting rules, element cycling, autocomplete, file I/O delegation |
| **Model** | `Script`, `Scene`, `ScriptElement` | Pure data, no Qt dependencies |
| **Services** | `ScriptManager`, `OsfSerializer`, `PdfExporter`, `CharacterRegister` | Persistence, export, auxiliary logic |

---

## 3. Class Diagram

```mermaid
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
```

---

## 4. Element Formatting Reference

These values are derived from industry standard (Trottier, 2014) and encoded
as constants returned by `ElementFormatting::forType()`.

| Element Type | Left Margin | Right Margin | Capitalization | Notes |
|---|---|---|---|---|
| `SCENE_HEADING` | 1.5 in | 1.0 in | ALL CAPS | e.g. `INT. OFFICE - DAY` |
| `ACTION` | 1.5 in | 1.0 in | Normal | Block description |
| `CHARACTER` | 3.7 in | 1.0 in | ALL CAPS | Centred above dialogue |
| `DIALOGUE` | 2.5 in | 2.5 in | Normal | Under character name |
| `PARENTHETICAL` | 3.1 in | 2.5 in | Normal | `(beat)` style |
| `TRANSITION` | 4.0 in | 1.0 in | ALL CAPS | e.g. `CUT TO:` |
| `SHOT` | 1.5 in | 1.0 in | ALL CAPS | Sub-heading |

---

## 5. Tab Key Element Cycling — State Diagram

Pressing **Tab** cycles the current element type following the industry-standard
screenplay flow. Pressing **Enter** on specific elements triggers a pre-defined
next element (e.g. Enter after a Character name jumps to Dialogue).

```mermaid
stateDiagram-v2
    direction LR

    [*] --> SCENE_HEADING : new document

    SCENE_HEADING --> ACTION : Tab
    ACTION --> CHARACTER : Tab
    CHARACTER --> DIALOGUE : Enter / Tab
    DIALOGUE --> CHARACTER : Enter
    DIALOGUE --> ACTION : Tab (double)
    CHARACTER --> PARENTHETICAL : ( key
    PARENTHETICAL --> DIALOGUE : Enter
    DIALOGUE --> TRANSITION : Tab (triple)
    TRANSITION --> SCENE_HEADING : Enter
    ACTION --> TRANSITION : Tab (triple)
    ACTION --> SCENE_HEADING : Enter (blank line)
```

### Key Press Decision Logic

```mermaid
flowchart TD
    A([Key Pressed]) --> B{Which key?}
    B -->|Tab| C{Current Element}
    B -->|Enter| D{Current Element}
    B -->|Open paren '('| E{Current Element is CHARACTER?}

    C -->|SCENE_HEADING| F[→ ACTION]
    C -->|ACTION| G[→ CHARACTER]
    C -->|CHARACTER| H[→ DIALOGUE]
    C -->|DIALOGUE| I[→ CHARACTER]
    C -->|PARENTHETICAL| J[→ DIALOGUE]
    C -->|TRANSITION| K[→ SCENE_HEADING]

    D -->|CHARACTER| L[→ DIALOGUE]
    D -->|DIALOGUE| M[→ CHARACTER]
    D -->|TRANSITION| N[→ SCENE_HEADING]
    D -->|blank ACTION| O[→ SCENE_HEADING]
    D -->|other| P[New line, same type]

    E -->|Yes| Q[→ PARENTHETICAL]
    E -->|No| R[Insert literal '(']
```

---

## 6. Sequence Diagrams

### 6.1 Application Startup

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant SVM as ScriptViewModel
    participant SM as ScriptManager
    participant Script

    User->>MW: launch application
    MW->>SVM: new ScriptViewModel()
    SVM->>SM: new ScriptManager()
    SM->>Script: new Script("Untitled", "")
    SM-->>SVM: manager ready
    SVM-->>MW: viewModel ready
    MW->>MW: setupUI()
    MW->>MW: connectSignals()
    MW-->>User: empty editor displayed
```

---

### 6.2 Save Script to OSF XML

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant SVM as ScriptViewModel
    participant SM as ScriptManager
    participant OSF as OsfSerializer
    participant FS as File System

    User->>MW: Ctrl+S
    MW->>SVM: saveScript(filePath)
    SVM->>SM: saveToFile(filePath)
    SM->>OSF: serialize(script)
    OSF->>OSF: open QXmlStreamWriter
    loop For each Scene
        OSF->>OSF: writeScene(scene)
        loop For each ScriptElement
            OSF->>OSF: writeElement(element)
        end
    end
    OSF-->>SM: QByteArray xmlData
    SM->>FS: write xmlData to .osf file
    FS-->>SM: success
    SM-->>SVM: true
    SVM-->>MW: emit scriptSaved()
    MW-->>User: status bar "Saved"
```

---

### 6.3 Load Script from OSF XML

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant SVM as ScriptViewModel
    participant SM as ScriptManager
    participant OSF as OsfSerializer
    participant CR as CharacterRegister
    participant SE as ScriptEditor

    User->>MW: File → Open
    MW->>MW: QFileDialog → filePath
    MW->>SVM: loadScript(filePath)
    SVM->>SM: loadFromFile(filePath)
    SM->>SM: read file to QByteArray
    SM->>OSF: deserialize(xmlData)
    OSF->>OSF: open QXmlStreamReader
    loop For each <scene> element
        OSF->>OSF: parseScene()
        loop For each child element
            OSF->>OSF: parseElement()
        end
    end
    OSF-->>SM: unique_ptr<Script>
    SM->>SM: store script
    SM-->>SVM: success
    SVM->>CR: buildFromScript(script)
    SVM->>SVM: emit scriptLoaded()
    SVM->>SE: refreshDisplay()
    SE-->>User: script rendered in editor
```

---

### 6.4 Tab Key Press — Element Cycling

```mermaid
sequenceDiagram
    actor User
    participant SE as ScriptEditor
    participant SVM as ScriptViewModel

    User->>SE: press Tab
    SE->>SE: keyPressEvent(Qt::Key_Tab)
    SE->>SVM: cycleElementType()
    SVM->>SVM: nextType = transitionTable[currentType]
    SVM->>SVM: currentType = nextType
    SVM->>SE: emit elementTypeChanged(nextType)
    SE->>SE: applyFormatting(nextType)
    SE-->>User: cursor position and margins updated
```

---

### 6.5 Character Autocomplete (SmartType)

```mermaid
sequenceDiagram
    actor User
    participant SE as ScriptEditor
    participant SVM as ScriptViewModel
    participant CR as CharacterRegister
    participant Popup as AutoCompletePopup

    User->>SE: types character name (e.g. "MA")
    SE->>SE: keyPressEvent — currentType is CHARACTER
    SE->>SVM: getSuggestions("MA")
    SVM->>CR: getSuggestions("MA")
    CR->>CR: filter names starting with "MA"
    CR-->>SVM: ["MARY", "MAX", "MARGARET"]
    SVM->>SE: emit autoCompleteReady(suggestions)
    SE->>Popup: show(suggestions)
    Popup-->>User: dropdown list shown

    User->>Popup: select "MARY"
    Popup->>SE: insertCompletion("MARY")
    SE->>SE: replace partial text
    SE-->>User: "MARY" inserted, cursor after name
```

---

### 6.6 Export to PDF

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant SVM as ScriptViewModel
    participant PX as PdfExporter
    participant Script
    participant FS as File System

    User->>MW: File → Export PDF
    MW->>MW: QFileDialog → pdfPath
    MW->>SVM: exportPdf(pdfPath)
    SVM->>PX: exportToPdf(script, pdfPath)
    PX->>PX: init QPdfWriter + QPainter
    PX->>PX: applyPageLayout() — Courier 12pt, A4/US Letter
    loop For each Scene
        loop For each ScriptElement
            PX->>PX: computeFormatting(element.type)
            PX->>PX: renderElement(painter, element, yPos)
            alt Page overflow
                PX->>PX: newPage()
            end
        end
    end
    PX->>FS: write PDF
    FS-->>PX: success
    PX-->>SVM: true
    SVM-->>MW: emit exportComplete()
    MW-->>User: status bar "PDF exported"
```

---

### 6.7 Navigate to Scene

```mermaid
sequenceDiagram
    actor User
    participant SN as SceneNavigator
    participant SVM as ScriptViewModel
    participant SE as ScriptEditor

    User->>SN: click scene in list
    SN->>SVM: getScenes()
    SVM-->>SN: QList<Scene*>
    SN->>SN: onSceneClicked(index)
    SN->>SE: scrollToScene(index)
    SE->>SE: find first element of scene in QTextDocument
    SE-->>User: view jumps to scene heading
```

---

## 7. Data Flow Diagram — Script Lifecycle

```mermaid
flowchart LR
    subgraph Input
        A[User Typing]
        B[OSF File on Disk]
    end

    subgraph ViewModel
        C[ScriptViewModel]
        D[CharacterRegister]
    end

    subgraph Model
        E[ScriptManager]
        F[Script]
        G[Scene]
        H[ScriptElement]
    end

    subgraph Serialisation
        I[OsfSerializer]
    end

    subgraph Output
        J[OSF XML File]
        K[PDF Document]
        L[ScriptEditor Display]
        M[SceneNavigator List]
    end

    A --> C
    B --> I --> E
    C --> E --> F --> G --> H
    H --> L
    G --> M
    F --> I --> J
    F --> K
    C --> D --> L
```

---

## 8. OSF XML Schema Overview

The application reads and writes the Open Screenplay Format (v4) XML structure.
The key mapping between the OSF XML elements and the application's model is:

```mermaid
flowchart TD
    Root["&lt;osf&gt;"]
    Meta["&lt;titlepage&gt;\n title, author, date"]
    Scenes["&lt;scene&gt; elements"]
    Heading["&lt;scene-heading&gt;\n → ElementType::SCENE_HEADING"]
    Action["&lt;action&gt;\n → ElementType::ACTION"]
    Character["&lt;character&gt;\n → ElementType::CHARACTER"]
    Dialogue["&lt;dialogue&gt;\n → ElementType::DIALOGUE"]
    Paren["&lt;parenthetical&gt;\n → ElementType::PARENTHETICAL"]
    Trans["&lt;transition&gt;\n → ElementType::TRANSITION"]
    Shot["&lt;shot&gt;\n → ElementType::SHOT"]

    Root --> Meta
    Root --> Scenes
    Scenes --> Heading
    Scenes --> Action
    Scenes --> Character
    Scenes --> Dialogue
    Scenes --> Paren
    Scenes --> Trans
    Scenes --> Shot
```

---

## 9. Sprint Plan Mapping to Architecture

| Sprint | Feature | Classes Involved |
|---|---|---|
| **Sprint 1** — Core Engine | Element typing, Tab cycling, formatting | `ScriptEditor`, `ScriptViewModel`, `ScriptElement`, `ElementFormatting` |
| **Sprint 2** — Navigation | Scene navigator panel | `SceneNavigator`, `ScriptViewModel`, `Scene` |
| **Sprint 3** — Serialisation | Open/Save OSF, PDF export | `ScriptManager`, `OsfSerializer`, `PdfExporter` |
| **Sprint 4** — Polish | Autocomplete, accessibility | `CharacterRegister`, WCAG compliance in `ScriptEditor` |

---

## 10. Technology Stack Summary

| Component | Technology | Rationale |
|---|---|---|
| Language | C++ 17 | Performance, memory control |
| Memory management | `std::unique_ptr`, `std::shared_ptr` | Safe ownership without GC overhead |
| UI Framework | Qt 6 Widgets | Cross-platform, low latency rendering |
| Text Engine | `QTextEdit` / `QTextDocument` | Handles long documents with custom formatting |
| XML I/O | `QXmlStreamReader` / `QXmlStreamWriter` | SAX-style, efficient for large OSF files |
| PDF Export | `QPdfWriter` + `QPainter` | Native Qt, no external dependency |
| Signals/Slots | Qt meta-object system | Decouples ViewModel from View |
| Build system | CMake | Cross-platform, Qt recommended |
| Diagrams | Mermaid.js | Version-controllable UML |
| Wireframes | Figma | WCAG 2.1 validation |

---

## 11. Design Patterns Applied

| Pattern | Where Used | Purpose |
|---|---|---|
| **MVVM** | Entire architecture | Decouple UI from business logic |
| **Strategy** | `ElementFormatting::forType()` | Swap formatting rules per element type |
| **Observer** (Qt Signals/Slots) | `ScriptViewModel` → View | Loose coupling between ViewModel and widgets |
| **Factory** (implicit) | `OsfSerializer::parseElement()` | Create correct `ScriptElement` subtype from XML tag |
| **Flyweight** | `ElementFormatting` | Share immutable formatting objects across elements |

---

*Diagrams rendered with [Mermaid.js](https://mermaid.js.org). All UML follows the project OSF v4 spec and Qt 6 architecture conventions.*
