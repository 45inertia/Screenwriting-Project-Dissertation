#ifndef SCRIPT_H
#define SCRIPT_H

#include "model/scene.h"
#include <QString>
#include <vector>

class Script {
private:
    QString title_;
    QString author_;
    std::vector<Scene> scenes_;

public:
    Script();

    // getters and setters
    QString getTitle() const;
    QString getAuthor() const;
    const std::vector<Scene>& getScenes() const;

    void setTitle(const QString newTitle);
    void setAuthor(const QString newAuthor);
    void addScene(const Scene newScene);
    void removeScene(int sceneNumber);
};

#endif // SCRIPT_H
