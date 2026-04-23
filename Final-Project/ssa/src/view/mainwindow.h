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

#include <QMainWindow>
#include <QLabel>
#include <QSplitter>

// forward declarations
class ScriptEditor;
class SceneNavigator;
class ScriptViewModel;
class ScriptManager;

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
    void onOpenScript();
    void onSaveScript();
    void onSaveAsScript();
    void onScriptTitleChanged(const QString& title);
    void onElementTypeChanged(ElementType type);
    void onStatusElementTypeClicked();
    void updateWordCount();
    void updateSceneIndicator(int sceneIndex);

private:
    Ui::MainWindow *ui;

    // core components
    ScriptManager* scriptManager_;
    ScriptViewModel* scriptViewModel_;
    ScriptEditor* scriptEditor_;
    SceneNavigator* sceneNavigator_;
    QSplitter* qSplitter_;

    // status bar labels
    QLabel* wordCountLabel_;
    QLabel* elementTypeLabel_;
    QLabel* sceneIndicatorLabel_;
    QLabel* pageCountLabel_;

    // Current save path
    QString currentFilePath_;

    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    QString elementTypeToString(ElementType type) const;

};
#endif // MAINWINDOW_H
