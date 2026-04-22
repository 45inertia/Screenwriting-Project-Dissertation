#include "model/script.h"
#include "model/scene.h"
#include "model/scriptelement.h"
#include <QtTest>
#include <memory>

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
    void scene_removeElement_decreaseCount();
    void scene_removeElement_nonExistentIdDoesNothing();

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
    ScriptElement testElement(1, ACTION, "Fade in on a desert.");
    QCOMPARE(testElement.getId(), 1);
    QCOMPARE(testElement.getType(), ACTION);
    QCOMPARE(testElement.getText(), QString("Fade in on a desert."));
}

void TestModelLayer::scriptElement_setText_updatesText() {
    ScriptElement testElement(1, DIALOGUE, "Hello.");
    testElement.setText("Goodbye.");
    QCOMPARE(testElement.getText(), QString("Goodbye."));
}

// ------------------------------
// SceneTests
// ------------------------------

void TestModelLayer::scene_defaultConstructor_hasZeroElements() {
    Scene scene;
    QCOMPARE(scene.getElementCount(), 0);
}

void TestModelLayer::scene_addElement_increasesCount() {
    Scene scene;
    scene.addElement(std::make_unique<ScriptElement>(1, ACTION, "Action line."));
    scene.addElement(std::make_unique<ScriptElement>(2, DIALOGUE, "Dialogue line."));
    QCOMPARE(scene.getElementCount(), 2);
}

void TestModelLayer::scene_getHeading_returnsCorrectHeading() {
    Scene scene(1, "INT. OFFICE - DAY");
    QCOMPARE(scene.getHeading(), QString("INT. OFFICE - DAY"));
}

void TestModelLayer::scene_setHeading_updatesHeading() {
    Scene scene(1, "INT. OFFICE - DAY");
    scene.setHeading("EXT. STREET - NIGHT");
    QCOMPARE(scene.getHeading(), QString("EXT. STREET - NIGHT"));
}

void TestModelLayer::scene_removeElement_decreaseCount() {
    Scene scene;
    scene.addElement(std::make_unique<ScriptElement>(1, ACTION, "Action line."));
    scene.addElement(std::make_unique<ScriptElement>(2, DIALOGUE, "Dialogue line."));
    scene.removeElement(1);
    QCOMPARE(scene.getElementCount(), 1);
}

void TestModelLayer::scene_removeElement_nonExistentIdDoesNothing() {
    Scene scene;
    scene.addElement(std::make_unique<ScriptElement>(1, ACTION, "Action line."));
    scene.removeElement(99);
    QCOMPARE(scene.getElementCount(), 1);
}

// ------------------------------
// ScriptTests
// ------------------------------

void TestModelLayer::script_defaultConstructor_hasCorrectDefaults() {
    Script script;
    QCOMPARE(script.getTitle(), QString("Untitled"));
    QCOMPARE(script.getAuthor(), QString("Unknown"));
    QCOMPARE(script.getSceneCount(), 0);
}

void TestModelLayer::script_addScene_increasesCount() {
    Script script;
    script.addScene(Scene(1, "INT. OFFICE - DAY"));
    script.addScene(Scene(2, "EXT. STREET - NIGHT"));
    QCOMPARE(script.getSceneCount(), 2);
}

void TestModelLayer::script_removeScene_decreasesCount() {
    Script script;
    script.addScene(Scene(1, "INT. OFFICE - DAY"));
    script.addScene(Scene(2, "EXT. STREET - NIGHT"));
    script.removeScene(1);
    QCOMPARE(script.getSceneCount(), 1);
}

void TestModelLayer::script_removeScene_nonExistentNumberDoesNothing() {
    Script script;
    script.addScene(Scene(1, "INT. OFFICE - DAY"));
    script.removeScene(99);
    QCOMPARE(script.getSceneCount(), 1);
}

void TestModelLayer::script_setTitle_updatesTitle() {
    Script script;
    script.setTitle("My Screenplay");
    QCOMPARE(script.getTitle(), QString("My Screenplay"));
}

void TestModelLayer::script_setAuthor_updatesAuthor() {
    Script script;
    script.setAuthor("Oliver Myers");
    QCOMPARE(script.getAuthor(), QString("Oliver Myers"));
}

QTEST_MAIN(TestModelLayer)
#include "testModelLayer.moc"

