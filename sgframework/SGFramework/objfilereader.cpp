#include "objfilereader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

OBJReader::OBJObject* OBJReader::sCurrentObject = 0;
OBJReader::OBJGroup* OBJReader::sCurrentGroup = 0;
OBJReader::OBJFile OBJReader::sCurrentFile;
QString OBJReader::sCurrentString;

bool OBJReader::loadOBJFile(const QString& filePath)
{
    QFile objFile(filePath);
    if (!objFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "OBJReader::loadOBJFile(): Could not open file " + filePath;
        return false;
    }
    QTextStream textStream(&objFile);
    sCurrentFile = OBJFile();
    sCurrentFile.mFileName = filePath;
    sCurrentGroup = 0;
    sCurrentObject = 0;
    qDebug() << "OBJReader::loadOBJFile(): Start Loading " + filePath;
    while (!textStream.atEnd())
    {
        sCurrentString = textStream.readLine();
        parseTextLine();
    }
    debugObjects();
    return true;
}

void OBJReader::parseTextLine()
{
    QString token = sCurrentString.section(' ', 0, 0);
    if (token == "#")
    {
        // If Comment read next Line
        return;
    }
    else if (token == "o")
    {
        // If "o" create new Object and read out name
        OBJObject newObject;
        sCurrentFile.mObjectList.append(newObject);
        sCurrentObject = &sCurrentFile.mObjectList.last();
        sCurrentGroup = 0;
        parseObject();
        return;
    }
    else if (token == "v")
    {
        parseVertex();
    }
    else if (token == "vt")
    {
        parseTexture();
    }
    else if (token == "vn")
    {
        parseNormals();
    }
    else if (token == "f")
    {
        if (!sCurrentObject)
        {
            OBJObject newObject;
            newObject.mObjectName = "NO_OBJECT";
            sCurrentFile.mObjectList.append(newObject);
            sCurrentObject = &sCurrentFile.mObjectList.last();
            sCurrentGroup = 0;
        }
        if (!sCurrentGroup)
        {
            OBJGroup newGroup;
            newGroup.mGroupName = "NO_GROUP";
            sCurrentObject->mGroupList.append(newGroup);
            sCurrentGroup = &sCurrentObject->mGroupList.last();
        }
        parseFaces();
    }
}

void OBJReader::parseObject()
{
    sCurrentObject->mObjectName = sCurrentString.section(' ', 1, 1);
}

void OBJReader::parseVertex()
{
    QString xCoordString, yCoordString, zCoordString, wCoordString;
    QString::SectionFlag flag = QString::SectionSkipEmpty;
    QVector4D vertex;
    float wCoord = 0.0f;

    xCoordString = sCurrentString.section(' ', 1, 1, flag);
    yCoordString = sCurrentString.section(' ', 2, 2, flag);
    zCoordString = sCurrentString.section(' ', 3, 3, flag);
    wCoordString = sCurrentString.section(' ', 4, 4, flag);

    if (wCoordString.isEmpty())
    {
        wCoord = 1.0f;
    }
    else
    {
        wCoord = wCoordString.toFloat();
    }

    vertex = QVector4D(xCoordString.toFloat(), yCoordString.toFloat(), zCoordString.toFloat(), wCoord);
    sCurrentFile.mVerticeList.append(vertex);
}

void OBJReader::parseTexture()
{
    QString uCoordString, vCoordString, wCoordString;
    QString::SectionFlag flag = QString::SectionSkipEmpty;
    QVector3D textur;
    uCoordString = sCurrentString.section(' ', 1, 1, flag);
    vCoordString = sCurrentString.section(' ', 2, 2, flag);
    wCoordString = sCurrentString.section(' ', 3, 3, flag);

    textur = QVector3D(uCoordString.toFloat(), vCoordString.toFloat(), wCoordString.toFloat());
    sCurrentFile.mTexturCoordList.append(textur);
}

void OBJReader::parseNormals()
{
    QString xCoordString, yCoordString, zCoordString;
    QString::SectionFlag flag = QString::SectionSkipEmpty;
    QVector3D normal;
    xCoordString = sCurrentString.section(' ', 1, 1, flag);
    yCoordString = sCurrentString.section(' ', 2, 2, flag);
    zCoordString = sCurrentString.section(' ', 3, 3, flag);

    normal = QVector3D(xCoordString.toFloat(), yCoordString.toFloat(), zCoordString.toFloat());
    sCurrentFile.mNormalList.append(normal);
}

void OBJReader::parseFaces()
{
    QStringList faceDefineStringList = sCurrentString.split(" ", QString::SkipEmptyParts);
    faceDefineStringList.removeAt(0);
    createTriangleFaces(faceDefineStringList);
}

void OBJReader::createTriangleFaces(QStringList& faceDefineStringList)
{
    if (faceDefineStringList.size() == 3)
    {
        QString vertexIndex, textureIndex, vormalIndex, temp;
        Face face;
        for (int i = 0; i < 3; i++)
        {
            temp = faceDefineStringList[i];
            vertexIndex = temp.section('/', 0, 0);
            textureIndex = temp.section('/', 1, 1);
            vormalIndex = temp.section('/', 2, 2);
            face.mVertexIndexList.append(vertexIndex.toInt() - 1);
            face.mTextureIndexList.append(textureIndex.toInt() - 1);
            face.mNormalIndexList.append(vormalIndex.toInt() - 1);
        }
        if (vormalIndex == "")
        {
            calculateNormals(face);
        }
        sCurrentGroup->mFaceList.append(face);
    }
    else if (faceDefineStringList.size() > 3)
    {
        QString vertexIndex, textureIndex, vormalIndex, temp;
        Face polygonFace;
        bool recalculateNormals = false;
        int j = faceDefineStringList.size();
        for (int i = 0; i < j; i++)
        {
            temp = faceDefineStringList[i];
            vertexIndex = temp.section('/', 0, 0);
            textureIndex = temp.section('/', 1, 1);
            vormalIndex = temp.section('/', 2, 2);
            polygonFace.mVertexIndexList.append(vertexIndex.toInt() - 1);
            polygonFace.mTextureIndexList.append(textureIndex.toInt() - 1);
            polygonFace.mNormalIndexList.append(vormalIndex.toInt() - 1);
            if (vormalIndex == "")
            {
                recalculateNormals = true;
            }
        }
        for (int i = 2; i < j; i++)
        {
            Face triangleFace;
            triangleFace.mVertexIndexList.append(polygonFace.mVertexIndexList[0]);
            triangleFace.mTextureIndexList.append(polygonFace.mTextureIndexList[0]);
            triangleFace.mNormalIndexList.append(polygonFace.mNormalIndexList[0]);
            triangleFace.mVertexIndexList.append(polygonFace.mVertexIndexList[i - 1]);
            triangleFace.mTextureIndexList.append(polygonFace.mTextureIndexList[i - 1]);
            triangleFace.mNormalIndexList.append(polygonFace.mNormalIndexList[i - 1]);
            triangleFace.mVertexIndexList.append(polygonFace.mVertexIndexList[i]);
            triangleFace.mTextureIndexList.append(polygonFace.mTextureIndexList[i]);
            triangleFace.mNormalIndexList.append(polygonFace.mNormalIndexList[i]);
            if (recalculateNormals)
            {
                calculateNormals(triangleFace);
            }
            sCurrentGroup->mFaceList.append(triangleFace);
        }
    }
    else
    {
        qDebug() << "[ERROR]OBJReader::createTriangleFaces(QStringList &faceDefineStringList): No 3D Faces defined, "
                    "Object is inconsistent!";
    }
}

void OBJReader::calculateNormals(Face& face)
{
    QVector3D vertices[3];
    vertices[0] = sCurrentFile.mVerticeList[face.mVertexIndexList[0]].toVector3D();
    vertices[1] = sCurrentFile.mVerticeList[face.mVertexIndexList[1]].toVector3D();
    vertices[2] = sCurrentFile.mVerticeList[face.mVertexIndexList[2]].toVector3D();

    vertices[0] = vertices[0] - vertices[1];
    vertices[2] = vertices[2] - vertices[1];
    vertices[1] = QVector3D::crossProduct(vertices[2], vertices[0]);

    sCurrentFile.mNormalList.append(vertices[1]);
    face.mNormalIndexList.clear();
    face.mNormalIndexList.append(sCurrentFile.mNormalList.size() - 1);
    face.mNormalIndexList.append(sCurrentFile.mNormalList.size() - 1);
    face.mNormalIndexList.append(sCurrentFile.mNormalList.size() - 1);
}

void OBJReader::debugObjects()
{
    qDebug() << "----------------OBJ FILE-------------------";
    qDebug() << "Nr. Objects:" << sCurrentFile.mObjectList.size();
    qDebug() << "Nr. Vertices:" << sCurrentFile.mVerticeList.size();
    qDebug() << "Nr. UV Coords:" << sCurrentFile.mTexturCoordList.size();
    qDebug() << "Nr. Normals:" << sCurrentFile.mNormalList.size();
    qDebug() << "-------------------------------------------";
}
