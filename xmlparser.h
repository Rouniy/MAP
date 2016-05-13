#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>

class XmlParser : public QObject
{
    Q_OBJECT
public:
    explicit XmlParser(QObject *parent = 0);
    QHash<QString, QString> Parse(const QString &fileName);
private:
    QXmlStreamReader xml;
    void ReadXml(const QString &fileName);
    void InitResult(QXmlStreamReader &xml, QHash<QString, QString> &results);
signals:

public slots:
};

#endif // XMLPARSER_H
