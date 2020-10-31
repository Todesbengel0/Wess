#include "audiolistener.h"
#include "audioengine.h"
#include "../SGFramework/sgobjectvisitor.h"

AudioListener::AudioListener()
{
  mOrientation.dir = QVector3D(0,0,1);
  mOrientation.up = QVector3D(0,1,0);
}

void AudioListener::setPosition(const QVector3D &position)
{
    mPosition = position;
}

QVector3D const& AudioListener::getPosition() const
{
    return mPosition;
}

void AudioListener::setOrientation(const Orientation &orientation)
{
    mOrientation = orientation;
}

Orientation const& AudioListener::getOrientation() const
{
    return mOrientation;
}

void AudioListener::setVelocity(const QVector3D &velocity)
{
    mVelocity = velocity;
}

QVector3D const& AudioListener::getVelocity() const
{
    return mVelocity;
}


void AudioListener::beforeTraverseChilds(SGObjectVisitor &visitor)
{
    SGPositionalObject::beforeTraverseChilds(visitor);

    mPosition= mWorldMatrix * QVector3D(0,0,0);
    mOrientation.up = mWorldMatrix * mOrientation.up;
    mOrientation.dir= mWorldMatrix * mOrientation.dir;
}
