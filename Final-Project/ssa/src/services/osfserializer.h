#ifndef OSFSERIALIZER_H
#define OSFSERIALIZER_H

#include "model/script.h"
#include "model/scene.h"
#include "model/scriptelement.h"
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <memory>

class OsfSerializer
{
public:
    OsfSerializer() = default;
    ~OsfSerializer() = default;

    bool save(const Script& script, const QString& filePath);
    std::unique_ptr<Script> load(const QString& filePath);

private:
    // static member functions so the function belongs to the class itself and not any instance
    // of it.
    static QString elementTypeToString(ElementType type);
    static ElementType stringToElementType(const QString& str);
};

#endif // OSFSERIALIZER_H
