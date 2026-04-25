#include "services/pdfexporter.h"
#include "model/scene.h"
#include "model/scriptelement.h"

#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QPageSize>
#include <QMarginsF>

bool PdfExporter::exportToPdf(const Script& script, const QString& filePath) {
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::Letter));
    writer.setPageMargins(QMarginsF(0, 0, 0, 0)); // handle margins manually
    writer.setResolution(72); // 72 dpi matches point measurements

    QPainter painter(&writer);
    if(!painter.isActive()) return false;

    // setting up the font
    QFont font("Courier New", 12);
    font.setFixedPitch(true);
    painter.setFont(font);

    QFontMetrics metrics(font, &writer);
    int lineHeight = metrics.height();
    int leading = metrics.leading();
    int lineSpacing = lineHeight + leading;

    // starting position
    int x = MARGIN_LEFT;
    int y = MARGIN_TOP + lineHeight;

    bool firstPage = true;

    auto checkNewPage = [&](int linesNeeded) {
        int requiredY = y + (linesNeeded * lineSpacing);
        if (!firstPage && requiredY > PAGE_HEIGHT - MARGIN_BOTTOM) {
            writer.newPage();
            y = MARGIN_TOP + lineHeight;
            return true;
        }
        firstPage = false;
        return false;
    };

    for(const Scene& scene : script.getScenes()) {
        // scene heading, two blank lines before bold
        QFont boldFont("Courier New", 12);
        boldFont.setBold(true);
        boldFont.setFixedPitch(true);
        painter.setFont(boldFont);

        checkNewPage(3);
        y += lineSpacing * 2; // two blank lines before scene heading

        QString heading = scene.getHeading().toUpper();
        painter.drawText(MARGIN_LEFT + OFFSET_ACTION, y, heading);
        y += lineSpacing;

        // reset to normal font
        painter.setFont(font);

        // draw elements
        for(const auto& element : scene.getElements()) {
            ElementType type = element->getType();
            QString text = element->getText();

            if (type == CHARACTER || type == TRANSITION || type == SHOT) {
                text = text.toUpper();
            }

            int xOffset = MARGIN_LEFT + getXOffset(type);
            int colWidth = getColumnWidth(type);
            QStringList lines = wrapText(text, colWidth, metrics);

            // spacing before element
            int spaceBefore = 0;
            if(type == CHARACTER) spaceBefore = 1;
            if(type == TRANSITION) spaceBefore = 1;
            if(type == SHOT) spaceBefore = 1;

            checkNewPage(lines.size() + spaceBefore);

            if(spaceBefore > 0) y += lineSpacing;

            for(const QString& line : lines) {
                painter.drawText(xOffset, y, line);
                y += lineSpacing;
            }

            // no gap between CHARACTER and DIALOGUE/PARENTHETICAL
            if(type != CHARACTER && type != PARENTHETICAL) {
                y += lineSpacing; // one blank line after all other elements
            }
        }



    }
    painter.end();
    return true;
}

int PdfExporter::getXOffset(ElementType type) const {
    switch(type) {
        case CHARACTER: return OFFSET_CHARACTER;
        case DIALOGUE: return OFFSET_DIALOGUE;
        case PARENTHETICAL: return OFFSET_PARENTHETICAL;
        case TRANSITION: return TEXT_WIDTH - OFFSET_TRANSITION;
        default: return OFFSET_ACTION;
    }
}

int PdfExporter::getColumnWidth(ElementType type) const {
    switch (type) {
        case DIALOGUE: return DIALOGUE_WIDTH;
        case PARENTHETICAL: return PARENTHETICAL_WIDTH;
        default: return TEXT_WIDTH;
    }
}

QStringList PdfExporter::wrapText(const QString &text, int columnWidth, const QFontMetrics &metrics) const {
    QStringList result;
    QStringList words = text.split(' ');
    QString currentLine;

    for(const QString& word : qAsConst(words)) {
        QString testLine = currentLine.isEmpty() ? word : currentLine + " " + word;

        if(metrics.horizontalAdvance(testLine) <= columnWidth) {
            currentLine = testLine;
        } else {
            if(!currentLine.isEmpty()) {
                result.append(currentLine);
            }
            currentLine = word;
        }
    }

    if(!currentLine.isEmpty()) {
        result.append(currentLine);
    }

    return result;
}


