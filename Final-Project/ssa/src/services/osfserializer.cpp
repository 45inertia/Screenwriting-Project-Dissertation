#include "osfserializer.h"
#include "model/script.h"
#include <QString>
#include <memory>

QString OsfSerializer::elementTypeToString(ElementType type) {
    switch(type) {
    case SCENE_HEADING:
        return "SCENE_HEADING";
    case ACTION:
        return "ACTION";
    case CHARACTER:
        return "CHARACTER";
    case DIALOGUE:
        return "DIALOGUE";
    case PARENTHETICAL:
        return "PARENTHETICAL";
    case TRANSITION:
        return "TRANSITION";
    case SHOT:
        return "SHOT";
    default:
        // action chosen as a fallback as it is the most neutral element type.
        return "ACTION";
    }
}

ElementType OsfSerializer::stringToElementType(const QString &str) {
    if (str == "SCENE_HEADING")
        return SCENE_HEADING;
    if (str == "CHARACTER")
        return CHARACTER;
    if (str == "DIALOGUE")
        return DIALOGUE;
    if (str == "PARENTHETICAL")
        return PARENTHETICAL;
    if (str == "TRANSITION")
        return TRANSITION;
    if (str == "SHOT")
        return SHOT;
    // ACTION returned by default so doesn't need to be checked.
    return ACTION;
}

bool OsfSerializer::save(const Script &script, const QString &filePath) {
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamWriter writer(&file);
    // writes human readable indented output.
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(4);

    writer.writeStartDocument();

    writer.writeStartElement("screenplay");
    writer.writeAttribute("title", script.getTitle());
    writer.writeAttribute("author", script.getAuthor());

    for(const Scene& scene : script.getScenes()) {
        // scene information
        writer.writeStartElement("scene");
        writer.writeAttribute("number", QString::number(scene.getSceneNumber()));
        writer.writeAttribute("heading", scene.getHeading());

        // scene elements information
        for(const auto & element : scene.getElements()) {
            writer.writeStartElement("element");
            writer.writeAttribute("type", elementTypeToString(element->getType()));
            writer.writeCharacters(element->getText());
            writer.writeEndElement(); // element
        }
        writer.writeEndElement(); // scene
    }

    writer.writeEndElement(); // screenplay
    writer.writeEndDocument();

    file.close();
    return true;

}

std::unique_ptr<Script> OsfSerializer::load(const QString &filePath) {
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }

    QXmlStreamReader reader(&file);

    // creating a Script object on the heap that is wrapped in unique_ptr and stored.
    auto script = std::make_unique<Script>();

    Scene* currentScene = nullptr;
    ElementType currentElementType = ACTION;
    int elementId = 0;

    while(!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();

        // functionality dependent on what the token type is
        switch(token) {
        case QXmlStreamReader::StartElement: {
            const QString name = reader.name().toString();

            if(name == "screenplay") {
                QString title = reader.attributes().value("title").toString();
                QString author = reader.attributes().value("author").toString();
                script->setTitle(title);
                script->setAuthor(author);
            }
            else if(name == "scene") {
                int number = reader.attributes().value("number").toInt();
                QString heading = reader.attributes().value("heading").toString();
                script->addScene(Scene(number, heading));

                // pointing the currentScene at the scene just added
                currentScene = &script->getScenes().back();
                elementId = 0;
            }
            else if(name == "element") {
                QString typeStr = reader.attributes().value("type").toString();
                currentElementType = stringToElementType(typeStr);
            }
            break;
        }
        case QXmlStreamReader::Characters: {
            // have to skip the whitespace only tokens produced by auto formatting
            QString text = reader.text().toString().trimmed();
            if(!text.isEmpty() && currentScene != nullptr) {
                currentScene->addElement(
                    std::make_unique<ScriptElement>(++elementId, currentElementType, text)
                    );
            }
            break;
        }
        case QXmlStreamReader::EndElement: {
            if(reader.name().toString() == "scene") {
                currentScene = nullptr;
            }
            break;
        }
        default:
            break;
        }
    }

    file.close();

    if(reader.hasError()) {
        return nullptr;
    }

    return script;

}


