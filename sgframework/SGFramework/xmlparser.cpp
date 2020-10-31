#include "xmlparser.h"
#include <QtCore>
#include <QtXml/QtXml>
#include <memory>
#include <utility>

XmlParser::XmlParser()
{

}

void XmlParser::readFile(CanvasRenderConfiguration *display, QString path, QString ID)
{
    QDomDocument document;
    QFile file(path);
    QString lError;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Didn't find Displayconfig in " << path;
        qDebug() << "Using default Displayconfig.";
        display->setParserData(defaultCanvas());
        return;
    }
    else
    {
        if(document.setContent(&file, &lError))
        {
            qDebug() << "Couldn't read XML File: " << lError;
            display->setParserData(defaultCanvas());
            return;
        }
        file.close();
    }
    QDomElement root = document.firstChildElement();

    QDomNodeList nodeList = root.elementsByTagName("displayconfig");
    for(int i = 0; i < nodeList.count(); ++i)
    {
        QDomNode node = nodeList.at(i);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            if(ID == element.attribute("id") || ID == "")
            {
                m_canvas.clear();
                parseFile(element,"canvas");
                display->setParserData(m_canvas);
                return;
            }
            else
            {
                m_canvas.clear();
                display->setParserData(defaultCanvas());
            }
        }
    }
}

void XmlParser::parseFile(QDomElement root, QString tagname)
{
    CanvasData values;
    QDomNodeList items = root.elementsByTagName(tagname);
    for(int i =0; i < items.count(); ++i)
    {
        QDomNode itemnode = items.at(i);
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
            values.width = itemele.attribute("width").toFloat();
            values.height = itemele.attribute("height").toFloat();
            splitString(itemele.attribute("leftBottomCorner"));
            values.leftBottom = QVector3D(0,0,0);
            values.leftTop = QVector3D(0,0,0);
            values.rightBottom = QVector3D(0,0,0);
            if(m_stringList.length() == 3 )
            values.leftBottom = QVector3D(m_stringList.at(0).toFloat(),m_stringList.at(1).toFloat(),m_stringList.at(2).toFloat());
            splitString(itemele.attribute("rightBottomCorner"));
            if(m_stringList.length() == 3)
            values.rightBottom = QVector3D(m_stringList.at(0).toFloat(),m_stringList.at(1).toFloat(),m_stringList.at(2).toFloat());
            splitString(itemele.attribute("leftUpperCorner"));
            if(m_stringList.length() == 3)
            values.leftTop = QVector3D(m_stringList.at(0).toFloat(),m_stringList.at(1).toFloat(),m_stringList.at(2).toFloat());
            values.angle = itemele.attribute("leftAngle").toFloat();
            values.leftCanvasId = itemele.attribute("leftCanvasId");
            values.id = itemele.attribute("id");
            values.hasCoordinates = true;
            if(values.leftBottom.isNull() == values.leftTop.isNull() == values.rightBottom.isNull())
            {
                values.hasCoordinates = false;
            }
            m_canvas.push_back(values);
        }
    }
}

void XmlParser::splitString(QString string)
{
    m_stringList.empty();
    m_stringList = string.split(",");
}

QVector<CanvasData> XmlParser::defaultCanvas()
{
    CanvasData data;
    data.id = "defaultCanvas";
    data.width = 3.0;
    data.height= 2.0;
    data.leftBottom = QVector3D(-4, -3.0, -5.0);
    data.leftTop = QVector3D(-4, 3.0, -5.0);
    data.rightBottom = QVector3D(4, -3.0, -5.0);
    data.hasCoordinates = true;
    m_canvas.push_back(data);
    return m_canvas;
}
