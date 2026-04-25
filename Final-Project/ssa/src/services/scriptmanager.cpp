#include "scriptmanager.h"
#include "model/script.h"
#include "services/osfserializer.h"
#include "services/pdfexporter.h"
#include <QObject>
#include <QString>
#include <memory>

ScriptManager::ScriptManager(QObject* parent)
    : QObject(parent),
    script_(nullptr),
    serializer_(std::make_unique<OsfSerializer>()),
    pdfExporter_(std::make_unique<PdfExporter>())
{}

Script* ScriptManager::getScript() const {
    return script_.get();
}

bool ScriptManager::hasScript() const {
    return script_ != nullptr;
}

void ScriptManager::newScript(const QString& title, const QString& author) {
    script_ = std::make_unique<Script>();
    script_->setTitle(title);
    script_->setAuthor(author);
    emit scriptLoaded();
}

bool ScriptManager::saveScript(const QString& filePath) {
    if(!hasScript()) {
        emit errorOccurred("No Script to save.");
        return false;
    }

    bool success = serializer_->save(*script_, filePath);
    if(success) {
        emit scriptSaved();
    } else {
        emit errorOccurred("Failed to save script to: " + filePath);
    }
    return success;
}

bool ScriptManager::loadScript(const QString& filePath) {
    auto loadedScript = serializer_->load(filePath);
    if(!loadedScript) {
        emit errorOccurred("Failed to load script from: " + filePath);
        return false;
    }

    script_ = std::move(loadedScript);
    emit scriptLoaded();
    return true;
}

void ScriptManager::closeScript() {
    script_.reset();
    emit scriptClosed();
}

bool ScriptManager::exportPdf(const QString &filePath) {
    if(!hasScript()) {
        emit errorOccurred("No script to export.");
        return false;
    }
    return pdfExporter_->exportToPdf(*script_, filePath);
}


