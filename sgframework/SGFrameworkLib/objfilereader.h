#ifndef OBJREADER_H
#define OBJREADER_H
#include <QList>
#include <QString>
#include <QVector4D>
#include <QVector3D>
#include <QStringList>

//! Liest mit Blender oder ähnlichen Modelern erzeugte .obj-Modelle ein. mtl-Dateien und einige weitere
//! Extensions werden dabei ignoriert. Normalen und Texturkoordinaten werden aber natürlich importiert.
class OBJReader
{
public:
    struct Face
    {
        QList<int> mVertexIndexList;
        QList<int> mTextureIndexList;
        QList<int> mNormalIndexList;
    };
    struct OBJGroup
    {
        QString mGroupName;
        QList<Face> mFaceList;
    };
    struct OBJObject
    {
        QString mObjectName;
        QList<OBJGroup> mGroupList;
    };
    struct OBJFile
    {
        QString mFileName;
        QList<QVector4D> mVerticeList;
        QList<QVector3D> mTexturCoordList;
        QList<QVector3D> mNormalList;
        QList<OBJObject> mObjectList;
    };
    static bool loadOBJFile(const QString& filePath);

private:
    static void parseTextLine();
    static void parseObject();
    static void parseVertex();
    static void parseTexture();
    static void parseNormals();
    static void parseFaces();
    static void createTriangleFaces(QStringList& faceDefineStringList);
    static void calculateNormals(Face& face);
    static void debugObjects();

public:
    static OBJFile sCurrentFile;

private:
    static OBJObject* sCurrentObject;
    static OBJGroup* sCurrentGroup;
    static QString sCurrentString;
};

#endif // OBJREADER_H
