#ifndef ICAMERAACCESS_H
#define ICAMERAACCESS_H

/*!
 * \author Sebastian Vesenmayer
 * \brief The ICameraAccess class
 * is the interface whic must be implemented to steer the camera from a dynamic character with cam
 */
class ICameraAccess
{
public:
    explicit ICameraAccess()
    {
    }
    virtual ~ICameraAccess()
    {
    }
    /*!
     * \brief setYAngleTripod
     * rotates arround a tripots y-axis with the angle in degrees
     * \param yAngle
     */
    virtual void setYAngleTripod(float yAngle) = 0;
    /*!
     * \brief setXAngleTripod
     * rotates arround a tripods x-axis with the angle in degrees
     * \param xAngle
     */
    virtual void setXAngleTripod(float xAngle) = 0;
    /*!
     * \brief setTranslationTripod
     * translates the tripod to the world position x,y,z
     * \param x
     * \param y
     * \param z
     */
    virtual void setTranslationTripod(float x, float y , float z) = 0;
};

#endif // ICAMERAACCESS_H
