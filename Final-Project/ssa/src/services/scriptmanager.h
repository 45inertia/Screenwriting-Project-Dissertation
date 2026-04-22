#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "model/script.h"
#include "services/osfserializer.h"
#include "services/pdfexporter.h"
#include <QObject>
#include <QString>
#include <memory>

class ScriptManager : public QObject {

    Q_OBJECT

public:
    explicit ScriptManager(QObject* parent = nullptr);
    ~ScriptManager() = default;

    // script access
    Script* getScript() const;
    bool hasScript() const;

    // script operations
    void newScript(const QString& title, const QString& author);
    bool saveScript(const QString& filePath);
    bool loadScript(const QString& filePath);
    void closeScript();

signals:
    void scriptLoaded();
    void scriptClosed();
    void scriptSaved();
    void errorOccurred(const QString& message);

private:
    std::unique_ptr<Script> script_;
    std::unique_ptr<OsfSerializer> serializer_;
    std::unique_ptr<PdfExporter> pdfExporter_;
};

#endif // SCRIPTMANAGER_H
