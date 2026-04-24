#include "scenenavigator.h"
#include "viewmodel/scriptviewmodel.h"

#include <QListWidgetItem>

SceneNavigator::SceneNavigator(ScriptViewModel* viewModel, QWidget *parent)
    : QWidget(parent),
    viewModel_(viewModel),
    sceneList_(nullptr),
    titleLabel_(nullptr)
{
    setupUi();

    // connecting to the viewModel signals
    connect(viewModel_, &ScriptViewModel::sceneListUpdated,
            this, &SceneNavigator::onSceneListUpdated);

    connect(viewModel_, &ScriptViewModel::currentSceneChanged,
            this, &SceneNavigator::onCurrentSceneChanged);

    connect(sceneList_, &QListWidget::itemClicked,
            this, &SceneNavigator::onItemClicked);
}



void SceneNavigator::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // title at the top of the navigator
    titleLabel_ = new QLabel("Scenes", this);

    // Scene List
    sceneList_ = new QListWidget(this);

    layout->addWidget(titleLabel_);
    layout->addWidget(sceneList_);
    setLayout(layout);
}

// ---------- slots ----------------------------------------------------------------------------


void SceneNavigator::onSceneListUpdated() {
    sceneList_->clear();

    const QStringList scenes = viewModel_->getSceneList();
    for(const QString& entry : scenes) {
        sceneList_->addItem(entry);
    }
}

void SceneNavigator::onCurrentSceneChanged(int sceneIndex) {
    // blocking signals to stop triggering onItemClicked when the selection is changed.
    sceneList_->blockSignals(true);
    sceneList_->setCurrentRow(sceneIndex);
    sceneList_->blockSignals(false);
}

void SceneNavigator::onItemClicked(QListWidgetItem *item) {
    int sceneIndex = sceneList_->row(item);
    viewModel_->onSceneSelected(sceneIndex);
}

// ---------- theming ----------------------------------------------------------------------------

void SceneNavigator::setDarkTheme() {
    setStyleSheet("QWidget { background-color: #252526; }");
    titleLabel_->setStyleSheet(
        "QLabel {"
        "  background-color: #2d2d2d;"
        "  color: #9d9d9d;"
        "  padding: 8px 12px;"
        "  font-family: 'Courier New';"
        "  font-size: 11px;"
        "  border-bottom: 1px solid #3d3d3d;"
        "}"
        );
    sceneList_->setStyleSheet(
        "QListWidget {"
        "  background-color: #252526;"
        "  color: #d4d4d4;"
        "  border: none;"
        "  font-family: 'Courier New';"
        "  font-size: 11px;"
        "}"
        "QListWidget::item {"
        "  padding: 6px 12px;"
        "  border-bottom: 1px solid #2d2d2d;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #094771;"
        "  color: #ffffff;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #2a2d2e;"
        "}"
        );
}

void SceneNavigator::setLightTheme() {
    setStyleSheet("QWidget { background-color: #f0f0f0; }");
    titleLabel_->setStyleSheet(
        "QLabel {"
        "  background-color: #e0e0e0;"
        "  color: #555555;"
        "  padding: 8px 12px;"
        "  font-family: 'Courier New';"
        "  font-size: 11px;"
        "  border-bottom: 1px solid #cccccc;"
        "}"
        );
    sceneList_->setStyleSheet(
        "QListWidget {"
        "  background-color: #f0f0f0;"
        "  color: #1e1e1e;"
        "  border: none;"
        "  font-family: 'Courier New';"
        "  font-size: 11px;"
        "}"
        "QListWidget::item {"
        "  padding: 6px 12px;"
        "  border-bottom: 1px solid #dddddd;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #0078d4;"
        "  color: #ffffff;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #e8e8e8;"
        "}"
        );
}

