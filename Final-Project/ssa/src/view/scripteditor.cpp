#include "scripteditor.h"
#include "viewmodel/scriptviewmodel.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QFont>
#include <QTextBlock>
#include <QMenu>
#include <QTextDocument>
#include <QTextFrame>

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
    currentElementType_(ACTION),
    isBulkOperation_(false)
{
    // Setting the font
    QFont font(ScreenplayFormat::FONT_FAMILY, ScreenplayFormat::FONT_SIZE);
    font.setFixedPitch(true);
    setFont(font);
    // constraining document to screenplay page width
    setLineWrapMode(QTextEdit::FixedPixelWidth);
    setLineWrapColumnOrWidth(816); // full 8.5 inch page at 96 DPI
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    applyDocumentMargins();

    // connecting element type changed
    connect(viewModel_, &ScriptViewModel::elementTypeChanged,
            this, &ScriptEditor::onElementTypeChanged);

    // connecting currentSceneChanged
    connect(viewModel_, &ScriptViewModel::currentSceneChanged,
            this, &ScriptEditor::onCurrentSceneChanged);

    // applying the initial formatting
    applyElementFormatting(ACTION);

    // connecting the cursor position to onCursorPositionChanged for updating
    // element type indicator
    connect(this, &QTextEdit::cursorPositionChanged,
            this, &ScriptEditor::onCursorPositionChanged);

    // for detecting if scene heading blocks diappear and updating the navigator
    connect(document(), &QTextDocument::contentsChanged,
            this, &ScriptEditor::onDocumentContentsChanged);
}

// ---- Syncing with the model -------------------------------------------------------------------

void ScriptEditor::syncToModel() {
    if (viewModel_->getSceneCount() == 0 && document()->isEmpty()) {
        return;
    }

    isBulkOperation_ = true;

    // collecting all blocks as (ElementType, text) pairs
    QList<QPair<ElementType, QString>> blocks;

    QTextBlock block = document()->begin();
    while(block.isValid()) {
        QString text = block.text().trimmed();
        ElementType type = getBlockElementType(block);
        if (!text.isEmpty()) {
            blocks.append(qMakePair(type, text));
        }
        block = block.next();
    }

    viewModel_->rebuildFromBlocks(blocks);
    isBulkOperation_ = false;
}

void ScriptEditor::loadFromScript() {
    // loading data from the data model through ScriptViewModel
    isBulkOperation_ = true;
    clear();
    applyDocumentMargins();

    int sceneCount = viewModel_->getSceneCount();
    if (sceneCount == 0) {
        return;
    }

    // get all content in block form from ScriptViewModel
    QList<QPair<ElementType, QString>> blocks = viewModel_->getAllBlocks();

    // If empty script then the first block is a scene heading
    if(blocks.isEmpty()) {
        applyElementFormatting(SCENE_HEADING);
        currentElementType_ = SCENE_HEADING;
        viewModel_->onElementTypeSelected(SCENE_HEADING);
        return;
    }

    QTextCursor cursor(document());
    bool firstBlock = true;
    // rebuilding the block map for scene navigation
    int blockNumber = 0;
    int sceneIndex = 0;

    for(const auto& pair : std::as_const(blocks)) {
        ElementType type = pair.first;
        QString text = pair.second;
        if (!firstBlock) {
            cursor.insertBlock();
            blockNumber++;
        }
        firstBlock = false;

        // applying the formatting for this element type
        cursor.setBlockFormat(blockFormatForType(type));
        cursor.setBlockCharFormat(charFormatForType(type));

        // store the element type on block
        cursor.block().setUserData(new BlockData(type));

        // insert the text;
        cursor.insertText(text);

        // registering the scene heading block positions
        if(type == SCENE_HEADING) {
            viewModel_->registerSceneBlock(sceneIndex, blockNumber);
            sceneIndex++;
        }
    }

    // moving the cursor the the start of the document
    QTextCursor start(document());
    setTextCursor(start);
    isBulkOperation_ = false;

    // triggering the cursor update after load
    onCursorPositionChanged();

}

void ScriptEditor::setBlockElementType(ElementType type) {
    // attach element type to current block as user data
    QTextBlock block = textCursor().block();
    block.setUserData(new BlockData(type));
}

ElementType ScriptEditor::getBlockElementType(const QTextBlock &block) const {
    BlockData* data = dynamic_cast<BlockData*>(block.userData());
    if (data) return data->elementType;
    return ACTION; // default if no data stored
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
        {
            // checking if the current block is empty
            QString currentText = textCursor().block().text().trimmed();
            if(currentText.isEmpty()) {
                showElementTypePicker();
                return;
            }

            // behaviour if block has content
            if(currentElementType_ == SCENE_HEADING) {
                QString heading = currentText.toUpper();

                int blockNumber = textCursor().blockNumber();
                int sceneIndex = viewModel_->getSceneCount();
                viewModel_->registerSceneBlock(sceneIndex, blockNumber);

                QTextEdit::keyPressEvent(event);
                //passing the actual heading text
                viewModel_->onNewSceneRequested(heading);
                return;
            }
            // insert a new block then report to viewModel
            QTextEdit::keyPressEvent(event);
            viewModel_->onEnterPressed();
            return;
        }

        default:
            // focing uppercase for element types that require it
            if (currentElementType_ == SCENE_HEADING ||
                currentElementType_ == CHARACTER ||
                currentElementType_ == TRANSITION) {

                if(event->text().length() == 1 && event->text().at(0).isLetter()) {
                    // inserting uppercase characters directly
                    QKeyEvent upperEvent(
                        event->type(),
                        event->key(),
                        event->modifiers(),
                        event->text().toUpper()
                    );
                    QTextEdit::keyPressEvent(&upperEvent);
                    return;
                }
            }
            QTextEdit::keyPressEvent(event);
            break;
        }
}

bool ScriptEditor::eventFilter(QObject *obj, QEvent *event) {
    // intercepting the Tab in the element picker menu to move to next item
    if(event->type() == QEvent::KeyPress) {
        QMenu* menu = qobject_cast<QMenu*>(obj);
        if(menu) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Tab) {
                // move to the next action
                QAction* current = menu->activeAction();
                QList<QAction*> actions = menu->actions();
                int idx = actions.indexOf(current);
                int next = (idx + 1) % actions.size();
                menu->setActiveAction(actions[next]);
                return true; // consume the tab
            }
        }
    }
    return QTextEdit::eventFilter(obj, event);
}

// popup type picker
void ScriptEditor::showElementTypePicker() {
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    const QList<ElementType> types = {
        SCENE_HEADING,
        ACTION,
        CHARACTER,
        DIALOGUE,
        PARENTHETICAL,
        TRANSITION,
        SHOT
    };

    // map from action to element type
    QMap<QAction*, ElementType> actionMap;

    for (ElementType type : types) {
        QAction* action = menu->addAction(elementTypeToDisplayString(type));
        actionMap[action] = type;

        // bolding the current type so the user knows where they are
        if (type == currentElementType_) {
            QFont f = action->font();
            f.setBold(true);
            action->setFont(f);
        }
    }

    // event filter on menu to handle tab key
    menu->installEventFilter(this);

    // positioning the menu at the cursor in the editor
    QRect cursorRect = this->cursorRect();
    QPoint globalPos = viewport()->mapToGlobal(cursorRect.bottomLeft());

    QAction* selected = menu->exec(globalPos);

    if (selected) {
        ElementType chosenType = actionMap[selected];
        // reformat the current block (not inserting a new block)
        viewModel_->onElementTypeSelected(chosenType);

    }


}

void ScriptEditor::onCursorPositionChanged() {

    if(isBulkOperation_) {
        return;
    }

    BlockData* data = dynamic_cast<BlockData*>(
        textCursor().block().userData());

    // only update if this block has explicitly stored element type data
    // new blocks created by Enter have no BlockData
    if (data) {
        currentElementType_ = data->elementType;
        viewModel_->onElementTypeSelected(data->elementType);
    }
}


QString ScriptEditor::elementTypeToDisplayString(ElementType type) const {
    switch (type) {
        case SCENE_HEADING:  return "Scene Heading";
        case ACTION:         return "Action";
        case CHARACTER:      return "Character";
        case DIALOGUE:       return "Dialogue";
        case PARENTHETICAL:  return "Parenthetical";
        case TRANSITION:     return "Transition";
        case SHOT:           return "Shot";
        default:             return "Action";
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

    // store the element type on this block
    setBlockElementType(type);
}

QTextBlockFormat ScriptEditor::blockFormatForType(ElementType type) const   {
    QTextBlockFormat format;

    switch(type) {
    case SCENE_HEADING:
        format.setLeftMargin(ScreenplayFormat::SCENE_HEADING_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        // two blank lines above scene heading, one below
        format.setTopMargin(24);
        format.setBottomMargin(12);
        break;

    case ACTION:
        format.setLeftMargin(ScreenplayFormat::ACTION_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        format.setTopMargin(0);
        break;

    case CHARACTER:
        format.setLeftMargin(ScreenplayFormat::CHARACTER_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        // blank line above character name
        format.setTopMargin(12);
        format.setBottomMargin(0);
        break;

    case DIALOGUE:
        format.setLeftMargin(ScreenplayFormat::DIALOGUE_LEFT);
        format.setRightMargin(ScreenplayFormat::DIALOGUE_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        format.setTopMargin(0);
        format.setBottomMargin(12);
        break;

    case PARENTHETICAL:
        format.setLeftMargin(ScreenplayFormat::PARENTHETICAL_LEFT);
        format.setRightMargin(ScreenplayFormat::PARENTHETICAL_RIGHT);
        format.setAlignment(Qt::AlignLeft);
        format.setTopMargin(0);
        format.setBottomMargin(12);
        break;

    case TRANSITION:
        format.setLeftMargin(ScreenplayFormat::TRANSITION_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignRight);
        format.setTopMargin(12);
        format.setBottomMargin(12);
        break;

    case SHOT:
        format.setLeftMargin(ScreenplayFormat::SHOT_LEFT);
        format.setRightMargin(ScreenplayFormat::DEFAULT_RIGHT);
        format.setAlignment(Qt::AlignRight);
        format.setTopMargin(12);
        break;
    }
    return format;
}

QTextCharFormat ScriptEditor::charFormatForType(ElementType type) const {
    QTextCharFormat format;
    QFont font(ScreenplayFormat::FONT_FAMILY, ScreenplayFormat::FONT_SIZE);
    font.setFixedPitch(true);

    if(type == SCENE_HEADING) {
        font.setBold(true);
    }

    format.setFont(font);
    format.setFontCapitalization(QFont::MixedCase);

    return format;
}

void ScriptEditor::applyDocumentMargins() {
    // page margins set on the document
    QTextFrameFormat frameFormat;
    frameFormat.setLeftMargin(144); // 1.5 inches at 96dpi
    frameFormat.setRightMargin(96); // 1 inch
    frameFormat.setTopMargin(96); // 1 inch top
    frameFormat.setBottomMargin(96); // 1 inch bottom
    document()->rootFrame()->setFrameFormat(frameFormat);
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

void ScriptEditor::onDocumentContentsChanged() {
    // count the scene heading blocks currently in the document

    if(isBulkOperation_) {
        return;
    }

    int headingCount = 0;
    QTextBlock block = document()->begin();
    while(block.isValid()) {
        if(getBlockElementType(block) == SCENE_HEADING &&
            !block.text().trimmed().isEmpty()) {
            headingCount++;
        }
        block = block.next();
    }

    // only syncing when a heading has been deleted
    if(headingCount < viewModel_->getSceneCount()) {
        syncNavigatorFromDocument();
    }

    // update navigator if an existing scene heading was edited
    int sceneIdx = 0;
    QTextBlock b = document()->begin();
    while (b.isValid() && sceneIdx < viewModel_->getSceneCount()) {
        if (getBlockElementType(b) == SCENE_HEADING &&
            !b.text().trimmed().isEmpty()) {
            viewModel_->updateSceneHeading(sceneIdx, b.text().trimmed().toUpper());
            sceneIdx++;
        }
        b = b.next();
    }
}

void ScriptEditor::syncNavigatorFromDocument() {
    // rebuild the scene list from document blocks without doing a full model rebuild
    QList<QPair<ElementType, QString>> blocks;
    QTextBlock block = document()->begin();
    while(block.isValid()) {
        QString text = block.text().trimmed();
        ElementType type = getBlockElementType(block);
        if(!text.isEmpty()) {
            blocks.append(qMakePair(type, text));
        }
        block = block.next();
    }
    viewModel_->rebuildFromBlocks(blocks);
}


