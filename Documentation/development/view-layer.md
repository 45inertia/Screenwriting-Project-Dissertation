# View Layer

*ScriptEditor, SceneNavigator, MainWindow decisions*

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