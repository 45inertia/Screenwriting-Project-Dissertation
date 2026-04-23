#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include "model/ElementType.h"
#include <QTextEdit>

// forward declaration as opposed to inlcude to avoid include chain.
class ScriptViewModel;

class ScriptEditor : public QTextEdit {

    Q_OBJECT

public:
    explicit ScriptEditor(ScriptViewModel* viewModel, QWidget *parent = nullptr);
    ~ScriptEditor() = default;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onElementTypeChanged(ElementType type);
    void onCurrentSceneChanged(int sceneIndex);

private:
    ScriptViewModel* viewModel_;
    ElementType currentElementType_;

    void applyElementFormatting(ElementType type);
    QTextBlockFormat blockFormatForType(ElementType type) const;
    QTextCharFormat charFormatForType(ElementType type) const;
    void scrollToBlock(int blockNumber);
};

#endif // SCRIPTEDITOR_H
