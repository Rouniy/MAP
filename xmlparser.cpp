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
    ReadXml(fileName);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "ROW")
            {
                InitResult(xml, results);
            }
        }
    }
    return results;
}

void XmlParser::InitResult(QXmlStreamReader &xml, QHash<QString, QString> &results)
{
    xml.readNext();
    QStringRef name;
    QStringRef code;
    QStringRef filetype;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement))
    {
        if (xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "CODE")
            {
                code = xml.text();
            }
            if (xml.name() == "NAME")
            {
                name = xml.text();
            }
            if (xml.name() == "SIGNFILETYPE")
            {
                filetype = xml.text();
            }
        }
        xml.readNext();
    }
    QString codeString = code.toString();
    QString nameString = name.toString() + filetype.toString();
    results[codeString] = nameString;
}

void XmlParser::ReadXml(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        xml.setDevice(&file);
    }
}
