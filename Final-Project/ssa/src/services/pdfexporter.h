#ifndef PDFEXPORTER_H
#define PDFEXPORTER_H

#include "model/script.h"
#include <QString>

class PdfExporter {

public:
    PdfExporter() = default;
    ~PdfExporter() = default;

    bool exportToPdf(const Script& script, const QString& filePath);
};

#endif // PDFEXPORTER_H
