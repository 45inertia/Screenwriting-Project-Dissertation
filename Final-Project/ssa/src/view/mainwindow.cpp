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
