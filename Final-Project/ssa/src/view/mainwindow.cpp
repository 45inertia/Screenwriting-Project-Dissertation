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

    scriptManager_ = new ScriptManager(this);
    scriptViewModel_ = new ScriptViewModel(scriptManager_, this);
    scriptEditor_ = new ScriptEditor(scriptViewModel_, this);

    scriptManager_->newScript("Test Script", "Oliver Myers");

    setCentralWidget(scriptEditor_);
    scriptEditor_->setPlaceholderText("Begin writing your screenplay...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::setupCentralWidget() {
//    qSplitter_ = new QSplitter(Qt::Horizontal, this);
//    sceneNavigator_ = new SceneNavigator(this);
//
//}

void MainWindow::setupToolBar() {
// TODO
}

void MainWindow::setupMenuBar() {
// TODO
}

void MainWindow::setupStatusBar() {
// TODO
}
