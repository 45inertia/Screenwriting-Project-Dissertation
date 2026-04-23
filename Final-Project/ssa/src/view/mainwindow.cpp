#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "view/mainwindow.h"
#include "ui_mainwindow.h"
#include "view/scripteditor.h"
#include "viewmodel/scriptviewmodel.h"
#include "services/scriptmanager.h"
#include "view/scenenavigator.h"

#include <QSplitter>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>

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

    // splitter
    qSplitter_ = new QSplitter(Qt::Horizontal, this);
    qSplitter_->addWidget(sceneNavigator_);
    qSplitter_->addWidget(scriptEditor_);

    // resiziing the splitter
    qSplitter_->setStretchFactor(0, 1);
    qSplitter_->setStretchFactor(1, 4);

    setCentralWidget(qSplitter_);

    scriptEditor_->setPlaceholderText("Begin writing your screenplay...");

    scriptManager_->newScript("Test Script", "Oliver Myers");
    scriptViewModel_->onNewSceneRequested();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupToolBar() {
// TODO
}

void MainWindow::setupMenuBar() {
// TODO
}

void MainWindow::setupStatusBar() {
// TODO
}
