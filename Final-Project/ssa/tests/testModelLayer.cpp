#include "model/script.h"
#include "model/scene.h"
#include "model/scriptelement.h"
#include <QtTest>

class TestModelLayer : public QObject {
    Q_OBJECT

private slots:
    // ScriptElement Tests
    void scriptElement_getters_returnConstructedValues();
    void scriptElement_setText_updatesText();

    // Scene Tests
    void scene_defaultConstructor_hasZeroElements();
    void scene_addElement_increasesCount();
    void scene_getHeading_returnsCorrectHeading();
    void scene_setHeading_updatesHeading();

    // Script Tests
    void script_defaultConstructor_hasCorrectDefaults();
    void script_addScene_increasesCount();
    void script_removeScene_decreasesCount();
    void script_removeScene_nonExistentNumberDoesNothing();
    void script_setTitle_updatesTitle();
    void script_setAuthor_updatesAuthor();
};

// ------------------------------
// ScriptElementTests
// ------------------------------

void TestModelLayer::scriptElement_getters_returnConstructedValues() {

}

void TestModelLayer::scriptElement_setText_updatesText() {

}

// ------------------------------
// SceneTests
// ------------------------------

void TestModelLayer::scene_defaultConstructor_hasZeroElements() {

}

void TestModelLayer::scene_addElement_increasesCount() {

}

void TestModelLayer::scene_getHeading_returnsCorrectHeading() {

}

void TestModelLayer::scene_setHeading_updatesHeading() {

}

// ------------------------------
// ScriptTests
// ------------------------------

void TestModelLayer::script_defaultConstructor_hasCorrectDefaults() {

}

void TestModelLayer::script_addScene_increasesCount() {

}

void TestModelLayer::script_removeScene_decreasesCount() {

}

void TestModelLayer::script_removeScene_nonExistentNumberDoesNothing() {

}

void TestModelLayer::script_setTitle_updatesTitle() {

}

void TestModelLayer::script_setAuthor_updatesAuthor() {

}

