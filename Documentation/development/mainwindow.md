#MainWindow

## `eventFilter`

The event filter is a way of intercepting events destined for another object before they reach it.

The `QLabel` for the status bar element type has no clicked signal as it is a display widget.
We need to make it clickable and be able to respond to mouse clicks. There are two options to do
this:
- Subclassing QLabel and overriding the `mousePressEvent`
- Installing an event filter on it from `MainWindow`

I decided to use an event filter as it looked simpler and keeps the MainWindow logic in one place.

```cpp
elementTypeLabel_->installEventFilter(this);
```
This tells the Qt that before delivering any event to `elementTypeLabel_` pass it to `this` 
(MainWindow) and let MainWindow decide what to do with it.

We check that the event is for the label and if it is a mouse click with:
```cpp
if (obj == elementTypeLabel_ && event->type() == QEvent::MouseButtonPress) {
```

The click is handled by showing the element type menu, returning true tells Qt that the event has
been handled and not to pass it onto the label. This prevents the label from trying to process the
click.

```cpp
onStatusElementTypeClicked();
return true;
```

Then for any other event on any other object it is passed up to the parent's class event filter
for normal processing. Without this then no event on any other object would work:

```cpp
return QMainWindow::eventFilter(obj, event);
```
