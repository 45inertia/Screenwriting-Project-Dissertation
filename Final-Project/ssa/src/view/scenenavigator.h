#ifndef SCENENAVIGATOR_H
#define SCENENAVIGATOR_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

// forward declaration as opposed to inlcude to avoid include chain.
class ScriptViewModel;

class SceneNavigator : public QWidget {

    Q_OBJECT

public:
    explicit SceneNavigator(ScriptViewModel* viewModel, QWidget *parent = nullptr);
    ~SceneNavigator() = default;

    void setDarkTheme();
    void setLightTheme();

private slots:
    void onSceneListUpdated();
    void onCurrentSceneChanged(int sceneIndex);
    void onItemClicked(QListWidgetItem* item);

private:
    ScriptViewModel* viewModel_;
    QListWidget* sceneList_;
    QLabel* titleLabel_;

    void setupUi();

};

#endif // SCENENAVIGATOR_H
