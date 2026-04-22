# Services Layer

*OsfSeraliser, ScriptManager, PdfExporter*

## Overview
The services layer sits between the model layer and the viewmodel layer. It is responsible for file 
persistence, script lifecycle management and PDF export. Unlike the model `ScriptManager` inherits
from `QObject` because it needs to notify the rest of the application when the script state changes.
`OsfSerializer` and `PdfExporter` remain as pure C__ as they perform pure data operations and have
no need for signals and slots.

## ScriptManager
The core responsibility of the `ScriptManager` is to own the active `Script` instance and provide a
clean interface to the rest of the application for creating, accessing and, and clearing scripts.
It sits between OsfSerializer and ScriptViewModel. 

**Singleton pattern rejected**

A singleton was considered and rejected. A singleton `ScriptManager` would be accessible globally.
This creates hidden dependencies that make the codebase harder to track and test. Instead one
instance is constructed at the application level and passed to what needs it. This dependency
injection pattern is considered better practice than global state.

**`ScriptManager` as `QObject`**

`ScriptManager` inherits from `QObject` and is the only service layer class to do so. This is
because `ScriptManager` needs to notify the `ScriptViewModel` when the script state changes. This
requires signals which require `QObject`. The alternatives were a callback pattern but signals were
preffered as they are consistent with the rest of the application's Qt-based communication.

**Ownership of `OsfSerializer` and `PdfExporter`**

`ScriptManager` owns both service objects as `unique_ptr` members. This means they are constructed
when the `ScriptManager` object is constructed and destroyed with it. The viewmodel layer requests
save, load and export operations through `ScriptManager` rather than instantiating serializers
and exporters directly. This means the file format concerns are kept to within the services layer
and means the viewmodel has no dependency on `OsfSerializer` or `PdfExporter` directly.

**`getScript()` returns a raw pointer**

This returns a raw pointer rather than a reference or a `unique_ptr`. Returning a `unique_ptr`
would transfer ownership out of `ScriptManager` which defeats the purpose of it managing the
lifetime. A reference implies the script always exists which is not true.

**Signals**

Four signals are defined:
- `scriptLoaded()` — emitted when a new or loaded script becomes active
- `scriptClosed()` — emitted when the active script is closed
- `scriptSaved()` — emitted on successful save
- `errorOccurred(QString)` — emitted with a message when any operation fails

These allow `ScriptViewModel` to react to state changes.

## `OsfSerializer`

What the serializer needs to handle in full:

**Writing**
1. Open a `QFile` at the given path for writing.
2. Attatch a `QXmlStreamWriter` to the file.
3. Enable Auto Formatting for readable output.
4. Write the `<screenplay>` root element with title and author attributes.
5. For each `<scene>` write the `<scene>` with number and heading attributes.
6. For each `<element>` in the `<scene>` write the `<element>` with type attribute and text content.
7. Close all elements and the document.
8. Close the file.

**Reading**
1. Open a `QFile` at the given path for reading.
2. Attatch a `QXmlStreamReader` to the file.
3. Loop through tokens with `readNext()`.
4. On `StartElement` named `screenplay` read title, author and create new `Script`.
5. On `StartElement` named `scene` read number, heading and create new `Scene`.
6. On `StartElement` named `element` read type attribute and store it temporarily.
7. On `Characters` (the element text) create `ScriptElement` with stored type and text.
8. On `EndElement` named `scene` add the completed `Scene` to the script.
9. On `EndDocument` or `error` finish and return.

**`QXmlStreamWriter` for saving**
`QXmlStreamWriter` writes XML token by token. `setAutoFormatting(true)` produces human readable
indented output useful for debugging and interoperability. Attributes must be written immediately
after `writeStartElement` before any child elements or text content. 

**`QXmlStreamReader` for loading**
`QXmlStreamReader` reads XML as a token stream using `readNext()`. The token types used are:
- `startElement`
- `characters`
- `endElement`
State is tracked across tokens using local variables. 

Whitespace-only characters produced by auto-formatted XML are filtered by calling `trimmed()` and
skipping empty strings.

**Element Type Conversion**
Two private static utility functions handle the conversion between the `ElementType` enums and its
string representation in XML. These are private static methods on `OsfSerializer` and nothing else
calls them.

**Error Handling**
Both `save` and `load` return early with `false` or `nullptr` if the file cannot be opened. After
reading, `reader.hasError()` is checked before returning. If the XML is corrupted in some way then
the load returns `nullptr` rather than a partially populated script.


## `PdfExporter`