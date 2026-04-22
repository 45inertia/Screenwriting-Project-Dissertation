#include "script.h"
#include "model/scene.h"
#include <QString>
#include <vector>
#include <algorithm>

Script::Script()
    : title_("Untitled"), author_("Unknown")
{}

QString Script::getTitle() const { return title_; }

QString Script::getAuthor() const { return author_; }

const std::vector<Scene>& Script::getScenes() const { return scenes_; }

int Script::getSceneCount() const { return static_cast<int>(scenes_.size()); }

void Script::setTitle(const QString& newTitle) { title_ = newTitle; }

void Script::setAuthor(const QString& newAuthor) { author_ = newAuthor; }

// pass by value (not const) as we want to use std::move which is in effect modifying.
void Script::addScene(Scene newScene) {
    scenes_.push_back(std::move(newScene));
}

//
void Script::removeScene(int sceneNumber) {
    auto it = std::find_if(scenes_.begin(), scenes_.end(),
                            // lambda function that captures the sceneNumber from the outer
                            // function so that it can be used inside.
                            // returns true for each scene in the vector if the scene numbers
                            // match.

                            // find_if stops and returns an iterator the the first scene where
                            // true is returned.
                            [sceneNumber](const Scene& s) {
                               return s.getSceneNumber() == sceneNumber;
        });
        // this as opposed to a manual index loop separates the search functionality with the
        // erase functionality.

    if (it != scenes_.end()) {
        scenes_.erase(it);
        // erase removes the elemtnt that the iterator points to and shifts everything after it
        // left reducing the vector size by one.
    }
}
