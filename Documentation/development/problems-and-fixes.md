# Problems and Fixes

*Things that didn't work and how they are solved*

### `script`
`elements_` was of type QList. QList uses implicit sharing (copy-on-write) internally which requires 
elements to be copyable. As `unique_ptr` is not copyable, `QList` and `unique_ptr` are not 
compatible. Due to this `elements_` was changed to `std::vector`. This also has the benefit of the
model layer being in pure C++ and not reliant on the Qt framework.

### Ownership Model for `scenes_` in `Script`: Plain Value vs `unique_ptr`

During the implementation of the `Script` class, a decision was required on how to store `Scene`
object in the `scenes_` vector. The initial implementation of `Scene::elements_` used
`std::vector<std::unique_ptr<ScriptElement>>`. This enforces single ownership of each
`ScriptElement`. The question was whether `Script` should apply the same pattern and store scenes
as `std::vector<std::unique_ptr<Scene>>` for consistency.

The arguement for `unique_ptr<Scene>` was that it makes ownership intent explicit in the type 
itself. However this was rejected for the following reasons:
- `unique_ptr` allocates each object individually on the heap at a location determined by the
  allocator. This means that the vector of pointers is contiguous in memory but the objects
  themselves are scattered. This means that accessing them requires pointer indirection which
  reduces cache locality. 
- For a plain value vector, all scenes are stored contiguously and neighbouring scenes can be loaded
  into cache simultaneously. In a screenplay application, the performance may be negligible in
  practice. There is no reason to deal with the cost of heap allocation and pointer indirection
  when it provides no benefit.
- The ownsership of each `Scene` is already enforced by containment: A `Scene` cannot exist in two
  `Script` objects simultaneously. 
- The consistency argument was considered. `unique_ptr` was used for `ScriptElement` because it
  was a technical requirement. `unique_ptr` here would add explicit ownership signalling at the
  cost of performance and verbosity. It also does not provide any ownership guarantee beyond what
  the vector does not already provide.

The final decision was to keep `Script` using `std::vector<Scene>` and document the reasons clearly.
`unique_ptr` is used where it solves a technical problem and plain value sematics are used where
containment is sufficient.

### `ScriptManager` QObject in the services layer
In principle it was determined the services layer should remain plain C++. However the need to emit
signals when the script state changes was the deciding factor in changing `ScriptManager` into a
`QObject`. This is documented as a deliberate exception to the plain C++ principle rather than
an oversight.

### `OsfSerializer` load function whitespace
Implementing the `writer.setAutoFormatting(true)` introduces whitespace in the xml file which causes
issues with the load operation. Therefore the whiespace needed to be skipped with `.trimmed()`.


### `OsfSerializer` load function `getScenes` overload
A pointer pointing to the last scene added is needed so that script elements can be added to this
scene. Currently `getScenes()` from Script is a const verison. A non const reference verison is
needed for the load operation so an overload function for `getScenes()` has been added.

### Connecting ScriptManager signals to ScriptViewModel without coupling the View
A design problem arose in connecting `ScriptManager`'s state change signals to the view layer.
One approach looked at was to connect the signals to slots in `ScriptEditor` and `Scenenavigator`.
However this would mean that the View layer has a dependency on `ScriptManager` which breaks the
MVVM boundary as the view should only communicate with the ViewModel.

The solution found was to connect `ScriptManager`'s signals to lambda functions inside 
`ScriptViewModel`'s constructor. Therefore when `ScriptManager` emits a relevant signal the
lambda resets `ScriptViewModel`'s state and emits the view facing signals so that the view only
connects to `ScriptViewModel` and does not know that `ScriptManager` exists.

All this together keeps the MVVM boundary intact as the ViewModel takes events from the services 
layer and translates them into view facing signals rather than letting the service layer signals
directly influence the view layer.

### `SceneNavigator` accessing `ScriptManager` directly MVVM boundary violation

During the implementation of `SceneNavigator`, the initial approach created a `ScriptViewModel`
member function called `viewModel_->getScriptManager()`. This meant that `SceneNavigator` was
going through the ViewModel into the services layer which violated the MVVM boundary. 

The fix was to remove the `getScriptManager` member function from `ScriptViewModel` and add two
ViewModel methods that provide exactly what the view needs which was `getSceneList()`. 

### Scene Navigation: Locating Scenes in a Continuous Document

A challenge arose when implementing the scene jump functinoality in `SceneNavigator`. 
The application needed to scroll `ScriptEditor` to the correct position when a user clicked a
scene in the navigator list. The problem was that `ScriptEditor` operates as a single continuous
QTextDocument with no concept of scenes. It is simply a sequence of paragraphs called blocks,
numbered from zero. There was no way to ask the document where do the scenes start.

The solution was to register each scene's starting block number in ScriptViewModel at the moment
the scene is created. When the user presses Enter to confirm a scene heading, keyPressEvent captures
the current block number before the new block is inserted and calls 
`ScriptViewModel::registerSceneBlock` with the same index and block number. They are stored in a
`QMap<int, int>` which maps scene index to block number.

When the user clicks a scene in SceneNavigator, ScriptViewModel emits currentSceneChanged with
the same index. ScriptEditor recieves this, looks up the block number via getBlockForScene and
calls scrollToBlock. This locates the block and moves the cursor there with setTextCursor, it calls
ensureCursorVisible to scroll the viewport.

There is a fallback of 0 in sceneBlockMap_.value(sceneIndex, 0) that is ensured if the scene has
not been registered. This means the editor scrolls to the top rather than producing undefined
behaviour.

### MainWindow event filter pattern

The status bar element type indicator needed to respond to mouse clicks to show the element type
picker menu. QLabel has no clicked signal as it is a display widget and not an interactive one.
Two options were considered:

1. Subclass QLabel and override mousePressEvent
2. Install an event filter on the label from MainWindow

The event filter approach was chosen as it keeps all MainWindow interaction logic in one place. 
`installEventFilter(this)` tells Qt to route all events destined for the label through MainWindow's
eventFilter method first. When a Mouse press event is detected on the label, the menu is shown and 
`return true` consumes the event. All other events return through the parent class eventFilter for
normal processing. Without the parent class fallback, all the events across all object would fail.

### Tab key not handled by QMenu

QMenu handles arrow key navigation natively but does not respond to Tab. The element type picker
in scriptEditor required Tab to cycle through options to match the Tab based workflow.

An event filter was installed on the menu object itself. When a KeyPress event with Qt::Key_Tab
is detected, the filter manually advances the active action.

The modulo operator is used to wrap the index back to zero when the last item is reached.

`qobject_cast<QMenu*>(obj)` checks the object type and returns nullptr if the object is not a QMenu.
This prevents the filter intercepting Tab on other objects.

### ScriptEditor and Script model disconnected
A gap was identified where the QTextEdit document and the Script data were completely disconnected.
The editor wrote to its own QTextDocument but this content was never transfered to ScriptElement
objects. When loaing, the Script model was populated from the OSF file but nothing used it to
populate the editor.

The solution was a two way mechanism for syncing.

syncToModel() reads every block in QTextDocument, extracts the stored ElementType and text, and 
rebuilds the Script model by clearing existing scenes and recreating them from the block data. This
is then called before every save.

loadFromScript() reads all scenes and elements from the Script model via getAllBlocks on the
ViewModel, clears the editor and repopulates it block by block applying the correct formatting and 
registering scene block positions for the navigator.

To know which ElementType each block holds, QTextBlockUserData was used which is Qt's mechanism for
attatching custom data to individual text blocks. A BlockData subclass stores the ElementType and is
set on each block whenever the element type changes via applyElementFormatting.
