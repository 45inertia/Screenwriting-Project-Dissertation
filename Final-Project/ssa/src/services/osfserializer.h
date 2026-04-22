#ifndef OSFSERIALIZER_H
#define OSFSERIALIZER_H

#include "model/script.h"
#include <QString>
#include <memory>

class OsfSerializer
{
public:
    OsfSerializer() = default;
    ~OsfSerializer() = default;

    bool save(const Script& script, const QString& filePath);
    std::unique_ptr<Script> load(const QString& filePath);
};

#endif // OSFSERIALIZER_H
