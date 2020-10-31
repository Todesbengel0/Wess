#include "fboproperty.h"

FBOProperty::FBOProperty()
{
    // setze höhere Priorität als Textur
    setPriority(67);
}

void FBOProperty::init(Node *renderTree, Drawable* ownerDrawable, int texDim, int unit, int pRes, QVector<TexImageData> pTexImageData)
{
    mRenderer = new FBORenderer();
    mRenderer->setRes(pRes);
    mRenderer->init(renderTree, texDim, ownerDrawable, pTexImageData);
    mTexID = mRenderer->getTexID();
    mTexDim = texDim;
    mTextureUnit = unit;
}

void FBOProperty::turnOn(Shader* shader)
{
    for (auto i=0; i < mTexID.length(); i++)
    {
        QString lName;
        lName = QString("textureMap").append(QString::number(i));

        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
        glFunctions->glBindTexture(mTexDim, mTexID[i]);
        shader->shaderProgram()->setUniformValue(lName.toStdString().c_str(), i + mTextureUnit);
        // due to a silly bug in apple's OGL-Driver no boolean uniforms work -> use float as a hack :(
        shader->shaderProgram()->setUniformValue((QString("has").append(lName)).toStdString().c_str(), 1.0f); // tell the shader that the current geometry
    }
}

void FBOProperty::turnOff(Shader* shader)
{
    for (auto i=0; i < mTexID.length(); i++)
    {
        QString lName;
        lName = QString("hastextureMap").append(QString::number(i));
        shader->shaderProgram()->setUniformValue(lName.toStdString().c_str(), 0.0f);
        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
        glFunctions->glBindTexture(mTexDim, 0);
    }
}

void FBOProperty::release()
{
}

FBOProperty::~FBOProperty()
{
    release();
}

DrawablePropertyId FBOProperty::getId() const
{
    return getPropertyId();
}
