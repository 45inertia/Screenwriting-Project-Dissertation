#include "scriptviewmodel.h"
#include "model/script.h"
#include "model/ElementType.h"
#include "services/scriptmanager.h"

ScriptViewModel::ScriptViewModel(ScriptManager* scriptManager, QObject* parent)
    : QObject(parent),
    scriptManager_(scriptManager), // pointer used to access scriptManager operations
    currentElementType_(ACTION),
    currentSceneIndex_(0)
{
    // reacting to ScriptManager state changes and resetting the ScriptViewModel state.
    // connecting scriptLoaded to the lambda
    connect(scriptManager_, &ScriptManager::scriptLoaded,
            this, [this]() {
                currentSceneIndex_ = 0;
                currentElementType_ = ACTION;
                emit sceneListUpdated(); // updating SceneNavigator
                emit currentSceneChanged(currentSceneIndex_); //
                emit scriptTitleChanged(scriptManager_->getScript()->getTitle());
            });

    // connecting scriptClosed to the lambda
    connect(scriptManager_, &ScriptManager::scriptClosed,
            this, [this]() {
                currentSceneIndex_ = 0;
                currentElementType_ = ACTION;
                emit sceneListUpdated();
            });

}

ElementType ScriptViewModel::getCurrentElementType() const {
    return currentElementType_;
}

int ScriptViewModel::getCurrentSceneIndex() const {
    return currentSceneIndex_;
}

QStringList ScriptViewModel::getSceneList() const {
    QStringList list;
    if(!scriptManager_->hasScript()) {
        return list;
    }

    for(const Scene& scene : scriptManager_->getScript()->getScenes()) {
        list << QString("%1. %2")
                .arg(scene.getSceneNumber())
                    .arg(scene.getHeading());
    }
    return list;
}

int ScriptViewModel::getSceneCount() const {
    if(!scriptManager_->hasScript()) {
        return 0;
    }
    return scriptManager_->getScript()->getSceneCount();
}

// ----- Tabbing Logic ----------------------------------------------------------------------------

void ScriptViewModel::onTabPressed() {
    currentElementType_ = cycleElementType(currentElementType_);
    emit elementTypeChanged(currentElementType_);
}

void ScriptViewModel::onEnterPressed() {

    // after character, moves to dialogue and vice versa, everything else should default to action.
    switch(currentElementType_) {

        case SCENE_HEADING:
            onNewSceneRequested();
            currentElementType_ = ACTION;
            break;
        case CHARACTER:
            currentElementType_ = DIALOGUE;
            break;
        case DIALOGUE:
            currentElementType_ = CHARACTER;
            break;
        case PARENTHETICAL:
            currentElementType_ = DIALOGUE;
            break;
        default:
            currentElementType_ = ACTION;
            break;
    }
        emit elementTypeChanged(currentElementType_);
}

ElementType ScriptViewModel::cycleElementType(ElementType current) const {
    switch(current) {
        case SCENE_HEADING:     return ACTION;
        case ACTION:            return CHARACTER;
        case CHARACTER:         return DIALOGUE;
        case DIALOGUE:          return CHARACTER;
        case PARENTHETICAL:     return DIALOGUE;
        case TRANSITION:        return SCENE_HEADING;
        case SHOT:              return ACTION;
        default:                return ACTION;
    }
}

// ---- Text Changes ------------------------------------------------------------------------------

void ScriptViewModel::onElementTextChanged(const QString &text) {
    // measures for no script or no scenes.
    if(!scriptManager_->hasScript()) {
        return;
    }
    if(scriptManager_->getScript()->getSceneCount() == 0 ) {
        return;
    }

    Script* script = scriptManager_->getScript();
    Scene& currentScene = script->getScenes()[currentSceneIndex_];

    if (currentScene.getElementCount() == 0) {
        currentScene.addElement(std::make_unique<ScriptElement>(1, currentElementType_, text));
    }
}

// -- Scene Operations ----------------------------------------------------------------------------

void ScriptViewModel::onSceneSelected(int sceneIndex) {
    if(!scriptManager_->hasScript()) {
        return;
    }

    int sceneCount = scriptManager_->getScript()->getSceneCount();
    if(sceneIndex < 0 || sceneIndex >= sceneCount) {
        return;
    }

    currentSceneIndex_ = sceneIndex;
    currentElementType_ = ACTION;
    emit currentSceneChanged(currentSceneIndex_);
    emit elementTypeChanged(currentElementType_);
}

void ScriptViewModel::onNewSceneRequested() {
    if(!scriptManager_->hasScript()) {
        return;
    }

    Script* script = scriptManager_->getScript();
    int newNumber = script->getSceneCount() + 1;
    script->addScene(Scene(newNumber, "INT. NEW SCENE - DAY"));

    currentSceneIndex_ = newNumber - 1;
    currentElementType_ = ACTION;

    emit sceneListUpdated();
    emit currentSceneChanged(currentSceneIndex_);
    emit elementTypeChanged(currentElementType_);

}

// --------Block Operations -----------------------------------------------------------------------

void ScriptViewModel::registerSceneBlock(int sceneIndex, int blockNumber) {
    sceneBlockMap_[sceneIndex] = blockNumber;
}

int ScriptViewModel::getBlockForScene(int sceneIndex) const {
    return sceneBlockMap_.value(sceneIndex, 0); // returns block number or 0 as fallback
}

// -----File Operations----------------------------------------------------------------------------

void ScriptViewModel::onNewScriptRequested(const QString &title, const QString &author) {
    scriptManager_->newScript(title, author);
    // scriptLoaded signal in ScriptManager triggers the lambda in the constructor
    // which handles the UI reset
}

void ScriptViewModel::onSaveRequested(const QString &filePath) {
    scriptManager_->saveScript(filePath);
}

void ScriptViewModel::onLoadRequested(const QString &filePath) {
    scriptManager_->loadScript(filePath);
}

// --- Element Type -------------------------------------------------------------------------------

void ScriptViewModel::onElementTypeSelected(ElementType type) {
    currentElementType_ = type;
    emit elementTypeChanged(type);
}



