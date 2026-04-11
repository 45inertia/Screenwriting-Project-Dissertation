#ifndef SCRIPTELEMENT_H
#define SCRIPTELEMENT_H

#include "model/ElementType.h"
#include <QString>

class ScriptElement
{
private:
    int id_;
    ElementType type_;
    QString text_;

public:
    // constructor and destructor
    ScriptElement(int id, ElementType type, const QString& text);
    ~ScriptElement();

    // getters and setters
    int getId() const;
    ElementType getElement() const;
    QString getText() const;
    void setText(const QString& text);

};

#endif // SCRIPTELEMENT_H
