#ifndef BULLETPHYSICMATRIXTOGEOMETRY_H
#define BULLETPHYSICMATRIXTOGEOMETRY_H

#include <QRunnable>
#include <QList>
#include "bulletphysicobject.h"

class BulletPhysicMatrixToGeometry : public QRunnable
{
public:
    BulletPhysicMatrixToGeometry(QList<BulletPhysicObject*> *a_BuPhObList,
                                 unsigned char a_ThreadId,
                                 unsigned char a_Threads);
    void run();
private:
    QList<BulletPhysicObject*>* m_BuPhObList;

    unsigned char m_ThreadId;
    unsigned char m_Threads;
};

#endif // BULLETPHYSICMATRIXTOGEOMETRY_H
