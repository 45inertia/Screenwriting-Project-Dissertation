# View Layer

*ScriptEditor, SceneNavigator decisions*

## ScriptEditor
This is the core writing widget. It needs to:
- Extend `QTextEdit` to get a full text editing surface
- Intercept Tab and Enter keys and report them to ScriptViewModel
- Apply formatting(font, margins, capitalisation) based on the current Element Type
- Display an indicator of the current element type

**Design Decisions**

`ScriptViewModel*` should be passed in through the construcor with a non owning raw pointer.

Formatting works by applying QTextBlockFormat and QTextCharFormat to the current block that
control the indentation, alignment and text transformation.

`ScriptEditor` can intercept key presses by overriding `keyPressEvent(QKeyEvent*)`. It will
check if the key is Tab or ender and call the appropriate ViewModel method and suppress the default
behaviour. All other keys pass through normally to `QTextEdit`

`ScriptEditor` should inherit from `QTextEdit` rather than containing one. This is because
subclassing gives direct access to `keyPressEvent`, the document and the cursor without any more
wiring.

**EventFilter**
```cpp
bool ScriptEditor::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QMenu* menu = qobject_cast<QMenu*>(obj);
        if (menu) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Tab) {
                QAction* current = menu->activeAction();
                QList<QAction*> actions = menu->actions();
                int idx = actions.indexOf(current);
                int next = (idx + 1) % actions.size();
                menu->setActiveAction(actions[next]);
                return true;
            }
        }
    }
    return QTextEdit::eventFilter(obj, event);
}
```

This is for the double enter key press that brings up the element type menu. `QMenu` handles arrow
keys natively but not Tab. This filter intercepts Tab key presses when the menu is the active
object, finds the currently highlighted action, calculates the next one using modulo arithmetic to 
wrap around at the end and sets it as active.

`return true` consumes the Tab so the menu doesn't try to process it further.

`qobject_cast<QMenu*>(obj)` returns `nullptr` if `obj` is not a `QMenu`. This means that the
`if (menu)` check safely handles any other object that might have this filter installed.

`(idx + 1) % actions.size()` In an example if the current index is 6(the last one),
`(6 + 1) % 7 = 0` which wraps around to the first item.

## SceneNavigator

