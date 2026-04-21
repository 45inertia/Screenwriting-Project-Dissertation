#include "script.h"
#include "model/scene.h"
#include <QString>
#include <vector>

Script::Script() {}

QString Script::getTitle() const { return title_; }

QString Script::getAuthor() const { return author_; }

const std::vector<Scene>& Script::getScenes() const { return scenes_; }

void Script::setTitle(const QString newTitle) { title_ = newTitle; }

void Script::setAuthor(const QString newAuthor) { author_ = newAuthor; }

void Script::addScene(const Scene newScene) { scenes_.push_back(newScene); }

void Script::removeScene(int sceneNumber) {}
