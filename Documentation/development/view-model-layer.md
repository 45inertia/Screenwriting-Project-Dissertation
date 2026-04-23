# ViewModel Layer

## `ScriptViewModel`
This is the bridge between the backend and the frontend. `ScriptViewModel` has the following
responsibilities:
- Hold a reference to `ScriptManager`.
- Epose the current script's data to the View layer.
- Handle the user actions from the View.
- Emit signals that the `ScriptEditor` and `SceneNavigator` can connect to.

A raw pointer to `ScriptManager` is held. This is so ownership remains at the applicaiton level
in `MainWindow`. Raw pointer is correct here because the lifetime of `ScriptManager` is guaranteed
to exceed that of `ScriptViewModel`.

**How does `ScriptViewModel` get `ScriptManager`?**

This should be passed in through the constructor.`ScriptViewModel` will not own `ScriptManager`
but it will hold a pointer to it.

**Tab key element type cycling**

When the user presses Tab in ScriptEditor, the element type should cycle in a context aware way
and follow screenplay conventions. This logic will live in the `ScriptViewModel` and not in the
`ScriptEditor`. 

**Enter Key Behaviour**

This mirrors the professional screenwriting applications where hitting enter decides inteligently 
what the next `ScriptElement` should be.

**Signals emited by `ScriptViewModel`**

The signals that should be emited are the following:
- `elementTypeChanged(ElementType)` tells `ScriptEditor` to reformat the current line.
- `sceneListUpdated()` tells `SceneNavigator` to refresh its list.
- `currentSceneChanged(int)` tells `SceneNavigator` which scene is selected.

**Signal connections in the constructor**

The constructor wires `ScriptManager`'s `scriptLoaded` and `scriptClosed` signals to lambda
functions. These reset the ViewModel state and emit the appropriate signals regarding the view
layer.