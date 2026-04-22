#ifndef SCENE_H
#define SCENE_H

#include "model/scriptelement.h"
#include <QString>
#include <vector>
#include <memory>

class Scene
{
private:
    int sceneNumber_;
    QString heading_;
    std::vector<std::unique_ptr<ScriptElement>> elements_;

public:
    Scene();
    Scene(int sceneNumber, const QString& heading);
    ~Scene() = default;

    // Scene is move only due to unique_ptr members
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    // getters and setters
    int getSceneNumber() const;
    const QString& getHeading() const;
    const std::vector<std::unique_ptr<ScriptElement>>& getElements() const;
    int getElementCount() const;

    void addElement(std::unique_ptr<ScriptElement> element);
    void setHeading(const QString& heading);

};

#endif // SCENE_H
