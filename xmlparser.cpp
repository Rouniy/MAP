#include "xmlparser.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QHash>

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{
}

QHash<QString, QString> XmlParser::Parse(const QString &fileName)
{
    QHash<QString, QString> results;
    if(fileName.size() == 0)
    {
        return results;
    }

    //
    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return results;
    }
    QXmlStreamReader xml(file);
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            QStringRef name = xml.name();
            if (name == "ROW")
            {
                InitResult(xml, name, results);
            }
        }
    }

    return results;
}

void XmlParser::InitResult(QXmlStreamReader &xml, const QStringRef &rootElementName, QHash<QString, QString> &results)
{
    xml.readNext();
    QString name;
    QString codeString;
    QString filetype;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == rootElementName))
    {
        if (xml.tokenType() == QXmlStreamReader::StartElement)
        {
            QStringRef elementName = xml.name();
            if (elementName == "CODE")
            {
                xml.readNext();
                codeString = xml.text().toString();
            }
            if (elementName == "NAME")
            {
                xml.readNext();
                name = xml.text().toString();
            }
            if (elementName == "SIGNFILETYPE")
            {
                xml.readNext();
                filetype = xml.text().toString();
            }
        }
        xml.readNext();
    }
    QString nameString = name + filetype;
    results[codeString] = nameString;
}
