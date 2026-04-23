#ifndef SCRIPTVIEWMODEL_H
#define SCRIPTVIEWMODEL_H

#include "model/script.h"
#include "model/ElementType.h"
#include "services/scriptmanager.h"
#include <QObject>
#include <QMap>

class ScriptViewModel : public QObject
{
    Q_OBJECT
public:
    // explicit prevents the compiler from using the constructor for implicit conversions.
    explicit ScriptViewModel(ScriptManager* scriptManager, QObject* parent = nullptr);
    ~ScriptViewModel() = default;

    // accessing current state
    ElementType getCurrentElementType() const;
    int getCurrentSceneIndex() const;
    QStringList getSceneList() const;
    int getSceneCount() const;

    // user actions from the view
    void onTabPressed();
    void onEnterPressed();
    void onElementTextChanged(const QString& text);
    void onSceneSelected(int sceneIndex);
    void onNewSceneRequested();

    // block operations
    void registerSceneBlock(int sceneIndex, int blockNumber);
    int getBlockForScene(int sceneIndex) const;

    // file operations
    void onNewScriptRequested(const QString& title, const QString& author);
    void onSaveRequested(const QString& filePath);
    void onLoadRequested(const QString& filePath);

signals:
    void elementTypeChanged(ElementType type);
    void sceneListUpdated();
    void currentSceneChanged(int sceneIndex);
    void scriptTitleChanged(const QString& title);

private:
    ScriptManager* scriptManager_;
    ElementType currentElementType_;
    int currentSceneIndex_;
    QMap<int, int> sceneBlockMap_; // scene index : block number

    ElementType cycleElementType(ElementType current) const;
};

#endif // SCRIPTVIEWMODEL_H
