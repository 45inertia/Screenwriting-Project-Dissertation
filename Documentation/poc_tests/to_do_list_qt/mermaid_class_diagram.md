[![](https://mermaid.ink/img/pako:eNp1VG1P2zAQ_isnf0BtSULaprSJUCUE-4A0NG1jY5oiIS82wcKxK9ttN1D57bPz0rql5Evsu-ce3z139isqJKEoQ2EY5sIww2kGd_JafmbawOViwVmBDZMCrjjWGq4ZLhWuclHjc1E469YI9qstcIuZuGeCyDW8Nnb3hXdYP99igUuqYFA1C8_94ybLvMjBku2cpztH7-s9IyU1MFhgRYXp71AXF5pLM59L8YAJccf1jnsVreSKOgD4iNPlgmBDXfU_GV13ro1fm1-EX9yKFkaqN2Pd-g3q34PH3OVTSGGldcsTMH1YSUY81C6vHhMGGOnDHym5h6CEmWNEBzCrj3PpXh8aYJueM76dwHvk9jy32dOMlQLz-bwu6ZIQSnq1bge6-6hvdRWko_wQ98nW8gFd3SiNV_SS82M95BITz7WBwxbt9aZJxDNoo5gooZ7492ZCdaHYwg2-53QKgzbYLLUnn6ddAD7tduexBeCR9PebYKjSZ7r5Hx28XxX_ThXDnL3sj96p42MF6M7ba09-ZJwKXNlU_O43s_luYloSm-1HNP09mm2GzcK7uDka5ggGYdiu_BuTgVyLVj_f7sfEUTRowyzejq-x3Ediomh-IIpFY841PEoFN1-aiH2Ei-mIFbV3XZ_ZH9EoQKViBGVGLWmAKqoq7Lao1jlH5olWNEeZXRKsnnOUi42NWWDxW8qqC1NyWT51m-YtaV_HLYIKQtWVXAqDsiRJ4poDZa_oL8qGk0mUJOlskp4P02ESn48C9A9l4WgWzaZxOk3HcRrPRvF4E6CX-thhFI_Px9Npkg5Hk3iWjJMAuRdCqtvmYa_f981_6JPWnQ?type=png)](https://mermaid.live/edit#pako:eNp1VG1P2zAQ_isnf0BtSULaprSJUCUE-4A0NG1jY5oiIS82wcKxK9ttN1D57bPz0rql5Evsu-ce3z139isqJKEoQ2EY5sIww2kGd_JafmbawOViwVmBDZMCrjjWGq4ZLhWuclHjc1E469YI9qstcIuZuGeCyDW8Nnb3hXdYP99igUuqYFA1C8_94ybLvMjBku2cpztH7-s9IyU1MFhgRYXp71AXF5pLM59L8YAJccf1jnsVreSKOgD4iNPlgmBDXfU_GV13ro1fm1-EX9yKFkaqN2Pd-g3q34PH3OVTSGGldcsTMH1YSUY81C6vHhMGGOnDHym5h6CEmWNEBzCrj3PpXh8aYJueM76dwHvk9jy32dOMlQLz-bwu6ZIQSnq1bge6-6hvdRWko_wQ98nW8gFd3SiNV_SS82M95BITz7WBwxbt9aZJxDNoo5gooZ7492ZCdaHYwg2-53QKgzbYLLUnn6ddAD7tduexBeCR9PebYKjSZ7r5Hx28XxX_ThXDnL3sj96p42MF6M7ba09-ZJwKXNlU_O43s_luYloSm-1HNP09mm2GzcK7uDka5ggGYdiu_BuTgVyLVj_f7sfEUTRowyzejq-x3Ediomh-IIpFY841PEoFN1-aiH2Ei-mIFbV3XZ_ZH9EoQKViBGVGLWmAKqoq7Lao1jlH5olWNEeZXRKsnnOUi42NWWDxW8qqC1NyWT51m-YtaV_HLYIKQtWVXAqDsiRJ4poDZa_oL8qGk0mUJOlskp4P02ESn48C9A9l4WgWzaZxOk3HcRrPRvF4E6CX-thhFI_Px9Npkg5Hk3iWjJMAuRdCqtvmYa_f981_6JPWnQ)

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
