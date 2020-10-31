#include <cassert>
#include <set>
#include <algorithm>
#include <ostream>
#include <QDir>

#include "trianglemesh.h"
#include "objfilereader.h"

TriangleMesh::TriangleMesh()
{
    mHasNormals = false;
    mHasTexCoords = false;
}

void TriangleMesh::init(const QString& filePath)
{
    mHasNormals = false;
    mHasTexCoords = false;
    objRead(filePath);
    this->copyDataFromObj();
    this->fillTris();
}

void TriangleMesh::objRead(QString filepath)
{
    QDir directory;
    for (int i = 0; i < 10; i++)
    {
        if (!directory.exists(filepath))
        {
            filepath.prepend("../");
        }
        else
        {
            break;
        }
    }

    if (!OBJReader::loadOBJFile(filepath))
    {
        qCritical() << "Failed to load " << filepath;
        return;
    }
}

void TriangleMesh::copyDataFromObj()
{
    if (OBJReader::sCurrentFile.mNormalList.size())
    {
        mHasNormals = true;
        mNorms = OBJReader::sCurrentFile.mNormalList;
    }
    if (OBJReader::sCurrentFile.mTexturCoordList.size())
    {
        mHasTexCoords = true;
        mTexCoords = OBJReader::sCurrentFile.mTexturCoordList;
    }
    mVerts = OBJReader::sCurrentFile.mVerticeList;
    qDebug() << "Reading obj-file finished.";
}

void TriangleMesh::fillTris()
{
    OBJReader::OBJFile* loaderObjFile = &OBJReader::sCurrentFile;

    // Iterate Over Objects
    for (int i = 0, j = loaderObjFile->mObjectList.size(); i < j; i++)
    {
        OBJReader::OBJObject* loaderObjObject = &loaderObjFile->mObjectList[i];

        // Iterate Over Groups
        for (int k = 0, l = loaderObjObject->mGroupList.size(); k < l; k++)
        {
            OBJReader::OBJGroup* loaderObjGroup = &loaderObjObject->mGroupList[k];

            // Read in Geometry Data and save it to the Meshes
            for (int m = 0, n = loaderObjGroup->mFaceList.size(); m < n; m++)
            {
                OBJReader::Face* loaderObjFace = &loaderObjGroup->mFaceList[m];
                for (int o = 0; o < 3; o++)
                {
                    int vertexLocation = loaderObjFace->mVertexIndexList[o];
                    int normalLocation = loaderObjFace->mNormalIndexList[o];
                    int texturLocation = loaderObjFace->mTextureIndexList[o];

                    mIVerts.append(vertexLocation);
                    mINorms.append(normalLocation);
                    mITexcoords.append(texturLocation);
                }
            }
        }
    }
}

void TriangleMesh::render(Drawable* /*d*/)
{
    glFunctions->glDrawArrays(GL_TRIANGLES, 0, mIVerts.size());
}

TriangleMesh::~TriangleMesh()
{
}

void TriangleMesh::fillVertices(QVector<QVector4D>& vertices)
{
    vertices.clear();
    int j;
    for (j = 0; j < mIVerts.size(); j++)
    {
        vertices.append(QVector4D(mVerts[mIVerts[j]].toVector3DAffine(), 1.0f));
    }

    qDebug() << "Rendering with " << j / 3 << " Tris.";
}

void TriangleMesh::fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& /*vertices*/)
{
    normals.clear();

    if (mHasNormals)
    {
        for (int j = 0; j < mINorms.size(); j++)
        {
            normals.append(mNorms[mINorms[j]]);
        }
    }
}

void TriangleMesh::fillTexCoords(QVector<QVector2D>& texCoords)
{
    texCoords.clear();

    if (mHasTexCoords)
    {
        for (int j = 0; j < mITexcoords.size(); j++)
        {
            texCoords.append(mTexCoords[mITexcoords[j]].toVector2D());
        }
    }
}

bool TriangleMesh::isTriangleList() const
{
    return true;
}
