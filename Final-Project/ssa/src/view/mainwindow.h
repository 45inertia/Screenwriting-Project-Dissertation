#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "view/scenenavigator.h"
#include "viewmodel/scriptviewmodel.h"
#include "services/scriptmanager.h"

// Forward declared here as only the pointers to them are needed and not the full functionality.
class ScriptEditor;
class ScriptNavigator;
class QSplitter;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewScript();


private:
    Ui::MainWindow *ui;
    ScriptEditor* scriptEditor_;
    ScriptViewModel* scriptViewModel_;
    ScriptManager* scriptManager_;
    SceneNavigator* sceneNavigator_;
    QSplitter* qSplitter_;
    QLabel* elementTypeLabel_;
    QLabel* pageCountLabel_;


    void setupCentralWidget();
    void setupToolBar();
    void setupMenuBar();
    void setupStatusBar();


};
#endif // MAINWINDOW_H
