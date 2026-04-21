#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "view/mainwindow.h"
#include "ui_mainwindow.h"
#include "view/scripteditor.h"
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
    setWindowTitle("SSA");
    resize(1280, 800);
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
    setupStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCentralWidget() {
    qSplitter_ = new QSplitter(Qt::Horizontal, this);
    sceneNavigator_ = new SceneNavigator(this);

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
