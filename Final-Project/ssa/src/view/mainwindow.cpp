#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/scripteditor.h"
#include "view/scenenavigator.h"
#include "view/newscriptdialog.h"
#include "viewmodel/scriptviewmodel.h"
#include "services/scriptmanager.h"
#include "model/ElementType.h"
#include "view/welcomescreen.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QPalette>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme_(false)
{
    ui->setupUi(this);
    setWindowTitle("SSA - Screenwriting Software");
    resize(1280, 800);

    // constructing object graph
    scriptManager_ = new ScriptManager(this);
    scriptViewModel_ = new ScriptViewModel(scriptManager_, this);
    scriptEditor_ = new ScriptEditor(scriptViewModel_, this);
    sceneNavigator_ = new SceneNavigator(scriptViewModel_, this);
    welcomeScreen_ = new WelcomeScreen(this);

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

    // loading the script from
    connect(scriptManager_, &ScriptManager::scriptLoaded,
            this, [this]() {
        centralStack_->setCurrentIndex(1); // switching from welcome to editor
        scriptEditor_->loadFromScript();
    });

    detectAndApplySystemTheme();

}

MainWindow::~MainWindow() {
    delete ui;
}

// ----setup--------------------------------------------------------------------------------------

void MainWindow::setupCentralWidget() {

    centralStack_ = new QStackedWidget(this);
    // page 0 is the welcome screen
    centralStack_->addWidget(welcomeScreen_);

    // splitter
    qSplitter_ = new QSplitter(Qt::Horizontal, this);
    qSplitter_->addWidget(sceneNavigator_);
    qSplitter_->addWidget(scriptEditor_);

    // resiziing the splitter
    qSplitter_->setStretchFactor(0, 1);
    qSplitter_->setStretchFactor(1, 4);

    centralStack_->addWidget(qSplitter_);

    setCentralWidget(centralStack_);
    // showing welcome screen on startup
    centralStack_->setCurrentIndex(0);

    connect(welcomeScreen_, &WelcomeScreen::newScriptRequested,
            this, &MainWindow::onNewScript);
    connect(welcomeScreen_, &WelcomeScreen::openScriptRequested,
            this, &MainWindow::onOpenScript);

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

    QMenu* viewMenu = menuBar()->addMenu("View");

    QAction* toggleThemeAction = viewMenu->addAction("Toggle Theme");
    toggleThemeAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_T));

    connect(toggleThemeAction, &QAction::triggered,
            this, &MainWindow::onToggleTheme);
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
        "  font-weight: bold;"
        "  font-family: 'Courier New';"
        "  color: #ffffff;"
        "  padding: 2px 8px;"
        "  border: 1px solid rgba(255,255,255,0.4);"
        "  border-radius: 3px;"
        "}"
        "QLabel:hover {"
        "  background-color: rgba(255,255,255,0.15);"
        "}"
    );
    elementTypeLabel_->setCursor(Qt::PointingHandCursor); // changes mouse cursor to hand pointer.
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
    if(dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString title = dialog.getTitle().trimmed();
    QString author = dialog.getAuthor().trimmed();

    if(title.isEmpty()) {
        title = "Untitled";
    }
    if(author.isEmpty()) {
        author = "Unknown";
    }

    QString deafultFilename = title.toLower()
                                  .replace(" ", "_")
                                  .remove(QRegularExpression("[^a-z0-9_]"))
                                + ".osf";

    QString defaultPath = QStandardPaths::writableLocation(
        QStandardPaths::DocumentsLocation);

    QString path = QFileDialog::getSaveFileName(
        this,
        "Save New Script",
        defaultPath,
        "Open Screenplay Format (*.osf"
    );

    if (path.isEmpty()) {
        return;
    }
    if(!path.endsWith(".osf", Qt::CaseInsensitive)) {
        path += ".osf";
    }

    // creatinig and saving
    currentFilePath_ = path;
    scriptViewModel_->onNewScriptRequested(title, author);
    scriptViewModel_->onSaveRequested(currentFilePath_);
    setWindowTitle("SSA - " + title);

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
    scriptEditor_->syncToModel();
    scriptViewModel_->onSaveRequested(currentFilePath_);
}

void MainWindow::onSaveAsScript() {
    QString path = QFileDialog::getSaveFileName(
        this,
        "Save Script As",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Open Screenplay Format (*.osf)"
    );

    if (path.isEmpty()) {
        return;
    }
    if(!path.endsWith(".osf", Qt::CaseInsensitive)) {
        path += ".osf";
    }

    currentFilePath_ = path;
    scriptEditor_->syncToModel();
    scriptViewModel_->onSaveRequested(path);

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

// ----- theming (light and dark mode)------------------------------------------------------------

void MainWindow::onToggleTheme() {
    isDarkTheme_ = !isDarkTheme_;
    if (isDarkTheme_) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void MainWindow::detectAndApplySystemTheme() {
    // Qt exposes the system palette and checks if the window background lightness is below
    // 128 which is considered a dark theme
    // 255 = white , 0 = black
    QPalette systemPalette = QApplication::palette();
    QColor windowColour = systemPalette.color(QPalette::Window);
    isDarkTheme_ = windowColour.lightness() < 128;

    if (isDarkTheme_) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void MainWindow::applyDarkTheme() {
    // global stylesheet for system-managed widgets
    qApp->setStyleSheet(
        "QMenuBar {"
        "  background-color: #2d2d2d;"
        "  color: #d4d4d4;"
        "}"
        "QMenuBar::item:selected {"
        "  background-color: #094771;"
        "}"
        "QMenu {"
        "  background-color: #2d2d2d;"
        "  color: #d4d4d4;"
        "  border: 1px solid #3d3d3d;"
        "}"
        "QMenu::item:selected {"
        "  background-color: #094771;"
        "}"
        "QStatusBar {"
        "  background-color: #007acc;"
        "  color: #ffffff;"
        "}"
        "QSplitter::handle {"
        "  background-color: #3d3d3d;"
        "}"
        "QMenu::item {"
        "  text-align: left;"
        "  padding: 4px 20px 4px 8px;"
        "}"



        //"QLabel {"
        //"  color: #ffffff;"
        //"}"
        );

    // status bar label colors are set explicitly not via global rule
    wordCountLabel_->setStyleSheet("color: #ffffff;");
    sceneIndicatorLabel_->setStyleSheet("color: #ffffff;");
    pageCountLabel_->setStyleSheet("color: #ffffff;");

    // update editor
    scriptEditor_->setStyleSheet(
        "QTextEdit {"
        "  background-color: #1e1e1e;"
        "  color: #d4d4d4;"
        "  border: none;"
        "}"
        );

    // updating widgets
    sceneNavigator_->setDarkTheme();
    welcomeScreen_->setDarkTheme();
}

void MainWindow::applyLightTheme() {
    qApp->setStyleSheet(
        "QMenuBar {"
        "  background-color: #f0f0f0;"
        "  color: #1e1e1e;"
        "}"
        "QMenuBar::item:selected {"
        "  background-color: #0078d4;"
        "  color: #ffffff;"
        "}"
        "QMenu {"
        "  background-color: #ffffff;"
        "  color: #1e1e1e;"
        "  border: 1px solid #cccccc;"
        "}"
        "QMenu::item:selected {"
        "  background-color: #0078d4;"
        "  color: #ffffff;"
        "}"
        "QStatusBar {"
        "  background-color: #0078d4;"
        "  color: #ffffff;"
        "}"
        "QSplitter::handle {"
        "  background-color: #cccccc;"
        "}"
        "QMenu::item {"
        "  text-align: left;"
        "  padding: 4px 20px 4px 8px;"
        "}"




        //"QLabel {"
        //"  color: #1e1e1e;"
        //"}"
        );

    // status bar label colors are set explicitly not via global rule
    wordCountLabel_->setStyleSheet("color: #ffffff;");
    sceneIndicatorLabel_->setStyleSheet("color: #ffffff;");
    pageCountLabel_->setStyleSheet("color: #ffffff;");


    scriptEditor_->setStyleSheet(
        "QTextEdit {"
        "  background-color: #ffffff;"
        "  color: #1e1e1e;"
        "  border: none;"
        "}"
        );

    sceneNavigator_->setLightTheme();
    welcomeScreen_->setLightTheme();
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
