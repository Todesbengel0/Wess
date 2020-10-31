/*
 *  texture.cpp
 *  heightmap_textured
 *
 *  Created by bdr on 27.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

//#include <cmath>
#include <qmath.h>
#include "drawable.h"
#include "texture.h"
#include <QImage>
#include <QDir>
#include <QColor>

void Texture::initMembers()
{
    mImage = NULL;
    mTexDim = GL_TEXTURE_2D;
    mMinFilter = GL_LINEAR_MIPMAP_LINEAR;
    mMaxFilter = GL_LINEAR;
    mWrapMode = GL_REPEAT;
    mInitialized = false;
    mForceInitialize = true;
    mTextureUnit = 0;
    mTexNameString = QString("textureMap");

    // setze Prio für das Sortieren der Shapelist
    setPriority(67);
}

void Texture::setDimMode(int mode)
{
    mTexDim = mode;
}

unsigned char* convertImageToByteArray(QImage* image)
{
    QImage imageMirrored = image->mirrored();

    unsigned char* imageBytes = imageMirrored.bits();

    unsigned long long numberOfBytes = image->height() * image->width() * 4;
    unsigned char* imageSwappedBytes = new unsigned char[numberOfBytes];
    unsigned long long vumberOfPixels = numberOfBytes / 4;
    numberOfBytes--;

    // Format von BGRA in RGBA umwandeln
    for (unsigned long long i = 0; i < vumberOfPixels; i++)
    {
        imageSwappedBytes[4 * i] = imageBytes[4 * i + 2];
        imageSwappedBytes[4 * i + 1] = imageBytes[4 * i + 1];
        imageSwappedBytes[4 * i + 2] = imageBytes[4 * i + 0];
        imageSwappedBytes[4 * i + 3] = imageBytes[4 * i + 3];
    }

    return imageSwappedBytes;
}

void Texture::buildMaps(bool buildmipmaps)
{
    GLenum faces[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

    if (mTexDim != GL_TEXTURE_CUBE_MAP)
    {
        unsigned char* bits = convertImageToByteArray(mImage);
        glFunctions->glTexImage2D(mTexDim, 0, GL_RGBA, mImage->width(), mImage->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
        delete bits;
    }
    else // GL_TEXTURE_CUBE_MAP
    {
        for (int i = 0; i < 6; i++)
        {
            unsigned char* bits = convertImageToByteArray(mImages[i]);
            glFunctions->glTexImage2D(faces[i], 0, GL_RGBA, mImages[i]->width(), mImages[i]->height(), 0, GL_RGBA,
                                      GL_UNSIGNED_BYTE, bits);
            delete bits;
        }
    }

    if (buildmipmaps)
    {
        glFunctions->glGenerateMipmap(mTexDim);
    }
}

void Texture::textureInit(bool buildmipmaps)
{
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);

    if (!mInitialized)
    {
        glFunctions->glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaximumAnisotropy);
        mAnisotropyLevel = mMaximumAnisotropy;

        glFunctions->glGenTextures(1, &mTexName);
        glFunctions->glBindTexture(mTexDim, mTexName);

        // optain the maximum texture size
        GLint texSize;
        glFunctions->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

        QImage* test = (mImage == NULL) ? mImages[0] : mImage;
        if (test == NULL)
        {
            qFatal("No valid texture image loaded.");
        }
        if (test->width() > texSize || test->height() > texSize)
        {
            qDebug() << "Texture is too large!";
            qDebug() << "Maximum dimension size is: " << texSize;
        }
        qDebug() << "Texture " << test->width() << " x " << test->height() << " loaded.";

        glFunctions->glGetError(); // clear the error flag

        this->buildMaps(buildmipmaps);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MAG_FILTER, mMaxFilter);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MIN_FILTER, mMinFilter);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_S, mWrapMode);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_T, mWrapMode);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_R, mWrapMode);

        glFunctions->glBindTexture(mTexDim, 0);
        mInitialized = true;
    }
}

void Texture::loadPicture(QString pfad)
{
    // Im Folgenden sind posy und negy ABSICHTLICH vertauscht:
    // Die Koordinatenachsen bei OpenGL Cubemaps stimmen nicht mit den typischen Cubemaps (Renderman, DirectX, ...)
    // überein
    // siehe hierzu GLSpec 4.5 Core, Seite 240

    QString fileNames[] = {"posx.png", "negx.png", "negy.png", "posy.png", "posz.png", "negz.png"};

    QDir directory;

    if (QDir::isRelativePath(pfad))
    {
        // try directories above... (macht nur sin bei relativen pfaden)
        for (int i = 0; i < 10; i++)
        {
            if (!directory.exists(pfad))
            {
                pfad.prepend("../");
            }
            else
            {
                break;
            }
        }
    }

    QDir folderOrFile(pfad);
    if (folderOrFile.count() > 0) // Versuche Cubemap zu laden
    {
        if (pfad[pfad.length()] != '/')
        {
            pfad.append("/");
        }
        for (int i = 0; i < 6; i++)
        {
            mImages[i] = new QImage(pfad + fileNames[i]);
            if (mImages[i]->isNull())
            {
                qDebug() << "Failure loading CubeMap " << fileNames[i] << " in " << pfad;
            }
        }
        mTexDim = GL_TEXTURE_CUBE_MAP;
#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
        glFunctions->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
    }
    else // TEXTURE_2D
    {
        mImage = new QImage(pfad);
        if (mImage->isNull())
        {
            qDebug() << "Failed to load " << pfad;
            makeDummy();
        }
    }
}

void Texture::makeStripes(int size, int thickness, bool buildmipmaps)
{
    int i, j;
    bool isWhite = true;

    delete mImage;
    mImage = new QImage(size, size, QImage::Format_RGBA8888);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            QColor color;
            if (j % thickness == 0)
            {
                isWhite = !isWhite;
            }
            if (isWhite)
            {
                color.setNamedColor(QString("white"));
            }
            else
            {
                color.setNamedColor(QString("black"));
            }
            mImage->setPixel(j, i, color.rgb());
        }
        isWhite = true;
    }

    this->textureInit(buildmipmaps);
}

void Texture::makeDummy()
{
    int i, j;
    float ti, tj;
    int height = 64, width = height;
    QColor color;

    delete mImage;
    mImage = new QImage(width, height, QImage::Format_RGBA8888);

    for (i = 0; i < width; i++)
    {
        ti = 2.0 * 3.14159265 * i / width;
        for (j = 0; j < height; j++)
        {
            tj = 2.0 * 3.14159265 * j / height;

            color.setRed((uchar)127 * (1.0 + sin(ti)));
            color.setGreen((uchar)127 * (1.0 + cos(2 * tj)));
            color.setBlue((uchar)127 * (1.0 + cos(ti + tj)));
            mImage->setPixel(i, j, color.rgb());
        }
    }
}

void Texture::turnOn(Shader* shader)
{
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
    if (mForceInitialize && !mInitialized)
    {
        textureInit();
    }

    glFunctions->glBindTexture(mTexDim, mTexName);
    shader->shaderProgram()->setUniformValue((mTexNameString.toStdString()).c_str(), (GLuint)mTextureUnit);
    // due to a silly bug in apple's OGL-Driver no boolean uniforms work -> use float as a hack :(
    shader->shaderProgram()->setUniformValue((QString("has").append(mTexNameString)).toStdString().c_str(), 1.0f); // tell the shader that the current geometry
                                                                  // has a texture
}

void Texture::turnOff(Shader* shader)
{
    shader->shaderProgram()->setUniformValue((QString("has").append(mTexNameString)).toStdString().c_str(), 0.0f);
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
    glFunctions->glBindTexture(mTexDim, 0);
}

Texture::~Texture()
{
    delete mImage;
    if (mTexName)
    {
        glFunctions->glDeleteTextures(1, &mTexName);
    }
}

DrawablePropertyId Texture::getId() const
{
    return getPropertyId();
}

bool Texture::hasAlphaChannel() const
{
    if (mImage)
    {
        return mImage->hasAlphaChannel();
    }

    if (mImages[0])
    {
        for (int i = 0; i < 6; ++i)
        {
            if (mImages[i]->hasAlphaChannel())
            {
                return true;
            }
        }

        return false;
    }

    return false;
}
