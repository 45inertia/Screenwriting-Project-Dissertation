#ifndef PDFEXPORTER_H
#define PDFEXPORTER_H

#include "model/script.h"
#include <QString>
#include <QFontMetrics>

class PdfExporter {

public:
    PdfExporter() = default;
    ~PdfExporter() = default;

    bool exportToPdf(const Script& script, const QString& filePath);

private:
    // page dimensions in points: 72pt = 1 inch
    static constexpr int PAGE_WIDTH = 612;
    static constexpr int PAGE_HEIGHT = 792;
    static constexpr int MARGIN_LEFT = 108;
    static constexpr int MARGIN_RIGHT = 72;
    static constexpr int MARGIN_TOP = 72;
    static constexpr int MARGIN_BOTTOM = 72;
    static constexpr int TEXT_WIDTH = 432;

    // element x offsets from the left margin
    static constexpr int OFFSET_ACTION = 0;
    static constexpr int OFFSET_CHARACTER = 144;
    static constexpr int OFFSET_DIALOGUE = 72;
    static constexpr int OFFSET_PARENTHETICAL = 108;
    static constexpr int OFFSET_TRANSITION = 0;
    static constexpr int OFFSET_SHOT = 0;

    // dialogue right boundary
    static constexpr int DIALOGUE_WIDTH = 288; // 4 inches
    static constexpr int PARENTHETICAL_WIDTH = 216; // 3 inches

    int getXOffset(ElementType type) const;
    int getColumnWidth(ElementType type) const;
    QStringList wrapText(const QString& text, int columnWidth,
                         const QFontMetrics& metrics) const;
};

#endif // PDFEXPORTER_H
