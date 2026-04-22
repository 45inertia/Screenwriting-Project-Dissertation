#ifndef SCRIPT_H
#define SCRIPT_H

#include "model/scene.h"
#include <QString>
#include <vector>
#include <algorithm>

class Script {
private:
    QString title_;
    QString author_;
    std::vector<Scene> scenes_;

public:
    Script();
    ~Script() = default;

    // Script is move only as a consequence of std::vector<Scene> being move only
    Script(Script&&) = default;
    Script& operator=(Script&&) = default;
    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;

    // getters and setters
    QString getTitle() const;
    QString getAuthor() const;
    const std::vector<Scene>& getScenes() const;
    // overload
    std::vector<Scene>& getScenes();
    int getSceneCount() const;

    void setTitle(const QString& newTitle);
    void setAuthor(const QString& newAuthor);
    void addScene(Scene newScene);
    void removeScene(int sceneNumber);
};

#endif // SCRIPT_H
