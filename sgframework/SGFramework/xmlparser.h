#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <QDebug>
#include <structure.h>
//#include "camera.h"
#include <QtXml/QtXml>
#include <QFile>
#include "canvasrenderconfiguration.h"

//! Ein Simpler XML-Parser welcher Daten aus einem XML-File liest
//! und die entsprenden Daten an die "DisplayConfiguration"
//! zurückgibt. Das XML-File wird aus dem Ordner SGFrameworkLib
//! geladen.

class XmlParser
{
public:
    XmlParser();

    //! Lädt das File und gibt die ausgelesen Daten an "DisplayConfiguration" zurück
    void readFile(CanvasRenderConfiguration* display, QString path, QString ID);
    //! Gibt eine Standardkonfigurierung zurück welche einen Bildschirm enthält
    QVector<CanvasData> defaultCanvas();

private:

    //!Startet den Parsing Vorgang
    void parseFile(QDomElement root, QString tagname);

    //! Splittet Vektoreingaben in einzelne Teilwerte
    void splitString(QString string);

    QVector <CanvasData> m_canvas;
    QStringList m_stringList;
};

#endif // XMLPARSER_H
