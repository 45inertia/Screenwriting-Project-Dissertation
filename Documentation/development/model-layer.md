# Model Layer

*Script, Scene, ScriptElement design decisions*

## Overview
The model layer consist of three classes:
- `ScriptElement`
- `Scene`
- `Script`
None of these inherit from `QObject`. This was a deliberate decision as now the model layer
represents pure data and has no dependency on Qt's UI framework. This means the data layer is
portable and testable independently of the UI.

Relevant members of these classes remain as `QString` to minimize lots of conversions from
`std::string` to `QString` and vice versa. `QString` is the string type that gets use with file
I/O and XML serialisation and data formatting as well as on the UI frontend. Converting adds
unnecessary friction with no architectural benefits. 

## `ScriptElement`
This represents a single unit of screenplay content e.g one line of dialogue, one scene heading etc.
It holds three pieces of data: an integer id, an `ElementType` enum and a `QString` of the text
content.

**`= default` destructor**

This destructor is declared `= default` rather than left out entirely to be explicit with intent.
`ScriptElement` does not contain resources that need manual cleanup so the compiler generated
destructor is correct.

`getType()` was renamed from `getElement()` for readability purposes.

## `Scene`

This represents one scene in the screenplay. It owns a `std::vector` of `ScriptElement` objects and
has a scene number and heading string.

**`std::vector` instead of `QList` for elements**

`QList` uses implicit sharing internally which requires all contained types to be copyable.
`unique_ptr` is not copyable by design leading to `QList` producing a compilation error. 
`std::vector` supports move-only types correctly and is consistent with `Script` which uses
`std::vector<Scene>`.

**The use of `unique_ptr`**

`ScriptElement` only belongs to one `Scene`. It should not be shared between scenes, copied
accidentally or exist without an owner. `unique_ptr` is better than raw pointers as the management
is automatic. This eliminates the need for manual memory management and simplifies ownership.
`QPointer` is a weak observer and does not extend to ownership which is why it has not been used
here. Also `ScriptElement` is not a `QObject` so `QPointer` would not compile here anyways.

**Move only sematics and the Rule of Five**

`unique_ptr` cannot be copied and therefore `Scene` cannot be copied either. The rule of five states
that if any one of the destructor, copy constructor, copy assignment, move constructor or move
assignment is defined then all five should be explicitly defined.

The copy versions are `= delete` which is forbidden and the move versions are `= default` which is
compiler generated. This means a scene can be transfered but never duplicated.

`std::move` is a cast that tells the compiler to treat an lvalue as an rvalue. Notably after a move
the source is left in a valid but empty state.

**`sceneNumber_` setter?**

Scene numbers are a consequence of position in the script and not an intrinsic property of a scene.
If a scene is inserted or removed then every following scene renumbers. If individual scenes were
allowed to set their own number then this would allow inconsistent states. Numbering is therefore
the responsibility of `Script` and not `Scene`. 

**getElements() const reference**
This has a const reference as the view layer should be able to iterate and read elements but not
modify or delete them directly. All mutation goes through `Scene` methods which keeps the data
integrity under `Scene`.

## `Script`
This is the top-level data object and owns all scenes and holds the title and author metadata.

**`std::vector` for scenes**

`std::vector` is the natural choice for an ordered sequence of move-only objects. It supports
`.push_back(std::move(...))` and does not require elements to be copyable. `std::vector` is also 
used rather than `QList` as it keeps `Script` freer of Qt dependencies which is consistent with the
model layer's design.

**`addScene` takes by value**
Taking `Scene` by value rather than `const Scene&` enables move sematics. A `const` reference
prevents moving from the argument since moving modifies the source. This gives the caller two paths:
- Pass a temporary: one move, zero copies
- Pass a named variable: caller explicitly uses `std::move` making the transfer visible.
In regards to memory management, moving a `Scene` into the function costs almost nothing because
the pointer to the vector's internal buffer is being transfered and the data is not being
duplicated.

**`removeScene` uses `std::find_if` with a lambda**
Rather than a manual index loop which was attempted. `std::find_if` expresses the intent clearly.
The lambda captures `sceneNumber` from the outer scope and if no match is found then `find_if`
returns `end()` and the erase is skipped.