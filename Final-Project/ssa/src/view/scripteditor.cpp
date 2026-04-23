#include "scripteditor.h"
#include "viewmodel/scriptviewmodel.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QFont>
#include <QTextBlock>

// Namespace containing all Industry Standard Screenplay Measurements
// All values in points (1 inch = 72 points)
// Page width assumed at 612pt (8.5 inches)
// Left margin base: 108 pt (1.5 inches)

namespace ScreenplayFormat {
    // left indentations
    constexpr int ACTION_LEFT = 0;
    constexpr int CHARACTER_LEFT = 144; // 2 inches from left margin
    constexpr int DIALOGUE_LEFT = 72; // 1 inch from left margin
    constexpr int PARENTHETICAL_LEFT = 108; // 1.5 inches from margin
    constexpr int TRANSITION_LEFT = 0;
    constexpr int SCENE_HEADING_LEFT = 0;
    constexpr int SHOT_LEFT = 0;

    // right indentations in points
    constexpr int DIALOGUE_RIGHT = 72;
    constexpr int PARENTHETICAL_RIGHT = 108;
    constexpr int DEFAULT_RIGHT = 0;

    // Font
    const QString FONT_FAMILY = "Courier New";
    constexpr int FONT_SIZE = 12;
}

ScriptEditor::ScriptEditor(ScriptViewModel *viewModel, QWidget *parent)
    : QTextEdit(parent),
    viewModel_(viewModel),
    currentElementType_(ACTION)
{
    // Setting the font
    QFont font(ScreenplayFormat::FONT_FAMILY, ScreenplayFormat::FONT_SIZE);
    font.setFixedPitch(true);
    setFont(font);

    // setting the dark background
    setStyleSheet("QTextEdit {"
                  "background-color: #1e1e1e;"
                  "color: #d4d4d4;"
                  "border:none;"
                  "}");

    // connecting to view model
    connect(viewModel_, &ScriptViewModel::elementTypeChanged,
            this, &ScriptEditor::onElementTypeChanged);

    // applying the initial formatting
    applyElementFormatting(ACTION);
}

// ---- Key Press Handling ------------------------------------------------------------------------

void ScriptEditor::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Tab:
        // reporting to the viewModel
        viewModel_->onTabPressed();
        return;
    case Qt::Key_Return:
    case Qt::Key_Enter:

        if(currentElementType_ == SCENE_HEADING) {
            int blockNumber = textCursor().blockNumber();
            int sceneIndex = viewModel_->getSceneCount();
            viewModel_->registerSceneBlock(sceneIndex, blockNumber);
        }
        // insert a new block then report to viewModel
        QTextEdit::keyPressEvent(event);
        viewModel_->onEnterPressed();
        return;

    default:
        // All other keys should pass normally
        QTextEdit::keyPressEvent(event);
        break;
    }
}


// ----- Formatting ------------------------------------------------------------------------------

void ScriptEditor::onElementTypeChanged(ElementType type) {
    currentElementType_ = type;
    applyElementFormatting(type);
}


void ScriptEditor::applyElementFormatting(ElementType type) {
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();

    // Applying Block format
    cursor.setBlockFormat(blockFormatForType(type));

    // applying char format
    // set on the block character format so the new text typed follows it
    cursor.setBlockCharFormat(charFormatForType(type));

    cursor.endEditBlock();
    setTextCursor(cursor);
}

QTextBlockFormat ScriptEditor::blockFormatForType(ElementType type) const   {
    QTextBlockFormat format;

    switch(type) {
    case SCENE_HEADING:
        format.setLeftMargin(ScreenplayFormat::SCENE_HEADING_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        break;

    case ACTION:
        format.setLeftMargin(ScreenplayFormat::ACTION_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        break;

    case CHARACTER:
        format.setLeftMargin(ScreenplayFormat::CHARACTER_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        break;

    case DIALOGUE:
        format.setLeftMargin(ScreenplayFormat::DIALOGUE_LEFT);
        format.setRightMargin(ScreenplayFormat::DIALOGUE_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        break;

    case PARENTHETICAL:
        format.setLeftMargin(ScreenplayFormat::PARENTHETICAL_LEFT);
        format.setRightMargin(ScreenplayFormat::PARENTHETICAL_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        break;

    case TRANSITION:
        format.setLeftMargin(ScreenplayFormat::TRANSITION_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignRight);
        break;

    case SHOT:
        format.setLeftMargin(ScreenplayFormat::SHOT_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignRight);
        break;
    }
    return format;
}

QTextCharFormat ScriptEditor::charFormatForType(ElementType type) const {
    QTextCharFormat format;
    QFont font(ScreenplayFormat::FONT_FAMILY, ScreenplayFormat::FONT_SIZE);
    font.setFixedPitch(true);

    switch(type) {
    case SCENE_HEADING:
        font.setBold(true);
        format.setFont(font);
        format.setFontCapitalization(QFont::AllUppercase);
        break;

    case CHARACTER:
        format.setFont(font);
        format.setFontCapitalization(QFont::AllUppercase);
        break;

    case TRANSITION:
        format.setFont(font);
        format.setFontCapitalization(QFont::AllUppercase);
        break;

    default:
        format.setFont(font);
        format.setFontCapitalization(QFont::MixedCase);
        break;
    }

    return format;
}

//------- Block Operations -----------------------------------------------------------------------

void ScriptEditor::onCurrentSceneChanged(int sceneIndex) {
    int blockNumber = viewModel_->getBlockForScene(sceneIndex);
    scrollToBlock(blockNumber);
}


void ScriptEditor::scrollToBlock(int blockNumber) {
    QTextDocument* doc = document();
    QTextBlock block = doc->findBlockByNumber(blockNumber);
    if (block.isValid()) {
        QTextCursor cursor(block);
        setTextCursor(cursor);
        ensureCursorVisible();
    }
}
