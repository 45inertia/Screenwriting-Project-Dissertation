#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/scripteditor.h"
#include "view/scenenavigator.h"
#include "view/newscriptdialog.h"
#include "viewmodel/scriptviewmodel.h"
#include "services/scriptmanager.h"
#include "model/ElementType.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SSA - Screenwriting Software");
    resize(1280, 800);

    // constructing object graph
    scriptManager_ = new ScriptManager(this);
    scriptViewModel_ = new ScriptViewModel(scriptManager_, this);
    scriptEditor_ = new ScriptEditor(scriptViewModel_, this);
    sceneNavigator_ = new SceneNavigator(scriptViewModel_, this);

    setupCentralWidget();
    setupMenuBar();
    setupStatusBar();

    // connecting ViewModel signals to MainWindow slots
    connect(scriptViewModel_, &ScriptViewModel::scriptTitleChanged,
            this, &MainWindow::onScriptTitleChanged);

    connect(scriptViewModel_, &ScriptViewModel::elementTypeChanged,
            this, &MainWindow::onElementTypeChanged);

    connect(scriptViewModel_, &ScriptViewModel::currentSceneChanged,
            this, &MainWindow::updateSceneIndicator);

    // update word count whenever the document changes
    connect(scriptEditor_->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::updateWordCount);

}

MainWindow::~MainWindow() {
    delete ui;
}

// ----setup--------------------------------------------------------------------------------------

void MainWindow::setupCentralWidget() {
    // splitter
    qSplitter_ = new QSplitter(Qt::Horizontal, this);
    qSplitter_->addWidget(sceneNavigator_);
    qSplitter_->addWidget(scriptEditor_);

    // resiziing the splitter
    qSplitter_->setStretchFactor(0, 1);
    qSplitter_->setStretchFactor(1, 4);

    setCentralWidget(qSplitter_);
    scriptEditor_->setPlaceholderText("Begin writing your screenplay...");
}


void MainWindow::setupMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");

    QAction* newAction = fileMenu->addAction("New Script");
    newAction->setShortcut(QKeySequence::New);

    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);

    QAction* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);

    QAction* saveAsAction = fileMenu->addAction("Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    fileMenu->addSeparator();

    QAction* quitAction = fileMenu->addAction("Quit");
    quitAction->setShortcut(QKeySequence::Quit);

    // connecting the actions to the functions
    connect(newAction, &QAction::triggered, this, &MainWindow::onNewScript);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenScript);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveScript);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAsScript);
    connect(quitAction, &QAction::triggered, this, &QApplication::quit);
}

// contains information on
// - word count
// - current ScriptElement
// - current Scene
// - page count
void MainWindow::setupStatusBar() {
    // word count
    wordCountLabel_ = new QLabel("Words: 0", this);
    wordCountLabel_->setMinimumWidth(100);
    statusBar()->addWidget(wordCountLabel_);

    // spacer to push element type to the centre as it is the most important and used
    QWidget* leftSpacer = new QWidget(this);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    statusBar()->addWidget(leftSpacer);

    // element type which is central and clickable
    elementTypeLabel_ = new QLabel("ACTION", this);
    elementTypeLabel_->setAlignment(Qt::AlignCenter);
    elementTypeLabel_->setMinimumWidth(160);
    elementTypeLabel_->setStyleSheet(
        "QLabel {"
        "font-weight: bold;"
        "font-family: 'Courier New';"
        "padding: 2px 8px;"
        "border: 1px solid palette(mid);"
        "border-radius: 3px;"
        "}"
        "QLabel:hover {"
        "background-color: palette(highlight);"
        "color: palette(highlighted-text);"
        "}"
    );
    elementTypeLabel_->setCursor(Qt::PointingHandCursor);
    elementTypeLabel_->installEventFilter(this);
    statusBar()->addWidget(elementTypeLabel_);

    // spacer to push scene and page to the right
    QWidget* rightSpacer = new QWidget(this);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    statusBar()->addWidget(rightSpacer);

    // Scene indicator
    sceneIndicatorLabel_ = new QLabel("Scene1", this);
    sceneIndicatorLabel_->setMinimumWidth(80);
    statusBar()->addPermanentWidget(sceneIndicatorLabel_);

    //page count
    pageCountLabel_ = new QLabel("Page 1", this);
    pageCountLabel_->setMinimumWidth(60);
    statusBar()->addPermanentWidget(pageCountLabel_);

}


// Event Filter for element type label click------------------------------------------------------

//bool MainWindow::eventFilter(QObject* obj, QEvent* event) {

//}


// ---- menu actions -----------------------------------------------------------------------------

void MainWindow::onNewScript() {
    NewScriptDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        currentFilePath_ = dialog.getFilePath();
        scriptViewModel_->onNewScriptRequested(dialog.getTitle(), dialog.getAuthor());

        // saving immediately to the chosen location
        scriptViewModel_->onSaveRequested(currentFilePath_);
        setWindowTitle("SSA - " + dialog.getTitle());
    }
}

void MainWindow::onOpenScript() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open Script",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Open Screenplay Format (*.osf)"
    );

    if(!path.isEmpty()) {
        currentFilePath_ = path;
        scriptViewModel_->onLoadRequested(path);
    }
}

void MainWindow::onSaveScript() {
    if(currentFilePath_.isEmpty()) {
        onSaveAsScript();
        return;
    }
    scriptViewModel_->onSaveRequested(currentFilePath_);
}

void MainWindow::onSaveAsScript() {
    QString path = QFileDialog::getSaveFileName(
        this,
        "Save Script As",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Open Screenplay Format (*.osf)"
    );

    if(!path.isEmpty()) {
        currentFilePath_ = path;
        scriptViewModel_->onSaveRequested(path);
    }
}

// --- status bar updates ------------------------------------------------------------------------

void MainWindow::onScriptTitleChanged(const QString &title) {
    setWindowTitle("SSA - " + title);
}

void MainWindow::onElementTypeChanged(ElementType type) {
    elementTypeLabel_->setText(elementTypeToString(type));
}

void MainWindow::onStatusElementTypeClicked() {
    QMenu menu(this);

    // list all element types
    const QList<ElementType> types = {
        SCENE_HEADING,
        ACTION,
        CHARACTER,
        DIALOGUE,
        PARENTHETICAL,
        TRANSITION,
        SHOT
    };

    // builds the element type menu list and adds the data of the type
    for (ElementType type : types) {
        QAction* action = menu.addAction(elementTypeToString(type));
        action->setData(static_cast<int>(type));
    }

    // show the menu below the label
    QAction* selected = menu.exec(
        elementTypeLabel_->mapToGlobal(
            QPoint(0, elementTypeLabel_->height())
        )
    );

    if (selected) {
        ElementType chosenType = static_cast<ElementType>(
            selected->data().toInt()
        );
        scriptViewModel_->onElementTypeSelected(chosenType);
    }
}

void MainWindow::updateWordCount() {
    QString text = scriptEditor_->toPlainText();
    int count = text.isEmpty() ? 0 :
                    text.split(QRegularExpression("\\s+"),
                                Qt::SkipEmptyParts).count();
    wordCountLabel_->setText(QString("Words: %1").arg(count));

    // A rough page estime of 250 words per screenplay page
    int pages = qMax(1, (count / 250) + 1);
    pageCountLabel_->setText(QString("Page %1").arg(pages));

}

void MainWindow::updateSceneIndicator(int sceneIndex) {
    int total = scriptViewModel_->getSceneCount();
    sceneIndicatorLabel_->setText(QString("Scene %1 of %2").arg(sceneIndex + 1).arg(total));
}

// --- helper utility ----------------------------------------------------------------------------

QString MainWindow::elementTypeToString(ElementType type) const {
    switch(type) {
        case SCENE_HEADING:  return "SCENE HEADING";
        case ACTION:         return "ACTION";
        case CHARACTER:      return "CHARACTER";
        case DIALOGUE:       return "DIALOGUE";
        case PARENTHETICAL:  return "PARENTHETICAL";
        case TRANSITION:     return "TRANSITION";
        case SHOT:           return "SHOT";
        default:             return "ACTION";
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(obj == elementTypeLabel_ && event->type() == QEvent::MouseButtonPress) {
        onStatusElementTypeClicked();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}
