---
title: ToDoList Application Class Diagram
---

classDiagram
    class MainWindow {
        -TaskManager *manager
        -UI::MainWindow *ui
        +MainWindow(QWidget *parent)
        <<slot>>on_addTask()
        <<slot>>on_removeTask ()
        <<slot>>updateListView()
    }
    class TaskManager {
        -vector~tasks~ tasks_
        +addTask(const Task& t) void
        +removeTask(int id) bool
        +editTask(const Task& t) bool
        +getTasks() const vector~Task~& 
        +getTask(int id) Task
        <<signal>> taskAdded(Task t)
        <<signal>> taskRemoved(int id)
        <<signal>> taskEdited(Task t)
        <<slot>>saveAll()
        <<slot>>loadAll()
    } 
    class Task {
        -int id
        -string title
        -string description
        -bool status
        +Task(int id, string title, string description, bool status)
        +getters/setters()
    }
    class XmlSerializer {
        +static serialize(string filename, vector~Task~ tasks) bool
        +static deserialize(string filename) vector~Task~
    }

    MainWindow "1" *-- "1" TaskManager : owns
    TaskManager "1" *-- "0..*" Task : contains
    TaskManager ..> XmlSerializer : calls for IO
    XmlSerializer ..> Task : creates/reads