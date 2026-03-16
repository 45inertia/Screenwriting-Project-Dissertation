#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>

class ScriptEditor : public QObject
{
    Q_OBJECT
public:
    explicit ScriptEditor(QObject *parent = nullptr);

signals:
};

#endif // SCRIPTEDITOR_H
