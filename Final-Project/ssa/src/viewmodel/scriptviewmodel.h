#ifndef SCRIPTVIEWMODEL_H
#define SCRIPTVIEWMODEL_H

#include <QObject>

class ScriptViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ScriptViewModel(QObject* parent = nullptr);
};

#endif // SCRIPTVIEWMODEL_H
