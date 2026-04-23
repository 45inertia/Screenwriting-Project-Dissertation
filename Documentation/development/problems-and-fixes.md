# Problems and Fixes

*Things that didn't work and how they are solved*

## Model Layer

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

## Services Layer

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

## ViewModel Layer

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