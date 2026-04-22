#include "scene.h"
#include "model/scriptelement.h"
#include <QString>
#include <vector>
#include <memory>

Scene::Scene()
    : sceneNumber_(0), heading_("INT. - DAY")
{}

Scene::Scene(int sceneNumber, const QString& heading)
    : sceneNumber_(sceneNumber), heading_(heading)
{}

int Scene::getSceneNumber() const {
    return sceneNumber_;
}

const QString& Scene::getHeading() const {
    return heading_;
}

const std::vector<std::unique_ptr<ScriptElement>>& Scene::getElements() const {
    return elements_;
}

int Scene::getElementCount() const {
    return elements_.size();
}

void Scene::addElement(std::unique_ptr<ScriptElement> element) {
    elements_.push_back(std::move(element));
}

void Scene::setHeading(const QString& heading) {
    heading_ = heading;
}
