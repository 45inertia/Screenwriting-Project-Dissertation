#include "scriptelement.h"

#include "model/ElementType.h"
#include <QString>

ScriptElement::ScriptElement(int id, ElementType type, const QString &text)
    : id_(id), type_(type), text_(text)
{}

int ScriptElement::getId() const { return id_; }

ElementType ScriptElement::getElement() const { return type_; }

QString ScriptElement::getText() const { return text_; }

void ScriptElement::setText(const QString& text) {
    text_ = text;
}
