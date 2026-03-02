# To Do List (Qt)

This document outlines the work gone into the production of the To Do List proof of concept project
in Qt

## Class Diagram



## Signals and Slots

It is vital to know what signals and slots should be present in the application.

A signal should represent a change in data or a request for a logical action.

**TaskManager**
Signals:
    - `taskAdded(Task t)`
    - `taskRemoved(int id)`
    - `taskEdited(Task t)`
    - `dataLoaded()` This is for when the data has been loaded and the  UI can update.


Slots:
    - `saveAll()` Triggered by UI or a timer to call XmlSerialization
    - `loadAll()` Triggered at ap startup to being the XML reading.

Public method:
    - `addTask(Task& t)`
    - `removeTask(int id)`  
    - `editTask(Task& t)`
**Note** that these are not slots because they emit the signal:
- *Ui calls public method*
- *Public method runs and emits signal on completion*
- *Ui elements react to signal being emitted*
