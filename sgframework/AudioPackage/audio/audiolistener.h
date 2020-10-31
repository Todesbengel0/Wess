#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H
#include <QVector3D>
#include "sgpositionalobject.h"

struct Orientation
{
    QVector3D dir;
    QVector3D up;
};
class AudioListener : public SGPositionalObject
{

public:
    AudioListener();
    //!
    //! \brief setPosition setzt neue Position
    //! \param position neue Position
    //!
    void setPosition(QVector3D const& position);
    //!
    //! \brief getPosition
    //! \return aktuelle Position
    //!
    const QVector3D& getPosition() const;
    //!
    //! \brief setOrientation setzt neue Orientierung
    //! \param orientation neue Orientierung
    //!
    void setOrientation(Orientation const& orientation);
    //!
    //! \brief getOrientation
    //! \return aktuelle orientierung
    //!
    const Orientation& getOrientation() const;
    //!
    //! \brief setVelocity setzt aktuelle Geschwindigkeit
    //! \param velocity neue Geschwindigkeit
    //!
    void setVelocity(QVector3D const& velocity);

    //!
    //! \brief getVelocity
    //! \return aktuelle Geschwindigkeit
    //!
    const QVector3D& getVelocity() const;

private:
    QVector3D mPosition;
    Orientation mOrientation;
    QVector3D mVelocity;

    // SGObject interface
protected:
    virtual void beforeTraverseChilds(SGObjectVisitor &visitor) override;
};

#endif // AUDIOLISTENER_H
