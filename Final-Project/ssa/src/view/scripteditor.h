#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include "model/ElementType.h"
#include <QTextEdit>
#include <QTextBlockUserData>

// stores the ElementType for each block in the QTextDocument
class BlockData : public QTextBlockUserData {
public:
    ElementType elementType;
    explicit BlockData(ElementType type) : elementType(type) {}
};

// forward declaration as opposed to inlcude to avoid include chain.
class ScriptViewModel;

class ScriptEditor : public QTextEdit {

    Q_OBJECT

public:
    explicit ScriptEditor(ScriptViewModel* viewModel, QWidget *parent = nullptr);
    ~ScriptEditor() = default;

    void syncToModel();
    void loadFromScript();


protected:
    void keyPressEvent(QKeyEvent* event) override;
    // overriding the event filter
    bool eventFilter(QObject* obj, QEvent* event) override;

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
    void showElementTypePicker();
    QString elementTypeToDisplayString(ElementType type) const;

    void setBlockElementType(ElementType type);
    ElementType getBlockElementType(const QTextBlock& block) const;
};

#endif // SCRIPTEDITOR_H
