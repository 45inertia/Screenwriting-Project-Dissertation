#include "osfserializer.h"
#include "model/script.h"
#include <QString>
#include <memory>

bool OsfSerializer::save(const Script &script, const QString &filePath) {
    // TODO: implement
    Q_UNUSED(script)
    Q_UNUSED(filePath)
    return false;
}

std::unique_ptr<Script> OsfSerializer::load(const QString &filePath) {
    // TODO: implement
    Q_UNUSED(filePath)
    return nullptr;
}
