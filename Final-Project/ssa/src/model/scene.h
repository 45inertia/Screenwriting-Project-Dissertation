#ifndef SCENE_H
#define SCENE_H

#include "model/scriptelement.h"
#include <QList>
#include <memory>

class Scene
{
private:
    int sceneNumber;
    QList<std::unique_ptr<ScriptElement>> elements;
public:
    Scene();
    ~Scene();

    int getSceneNumber() const;
    QString getHeading() const;
    QList<ScriptElement*> getElements() const;
};

#endif // SCENE_H
