#include "bulletphysicmatrixtogeometry.h"

BulletPhysicMatrixToGeometry::BulletPhysicMatrixToGeometry(QList<BulletPhysicObject*> *a_BuPhObList, unsigned char a_ThreadId, unsigned char a_Threads)
{
    m_BuPhObList=a_BuPhObList;
    m_ThreadId=a_ThreadId;
    m_Threads=a_Threads;

    this->setAutoDelete(false);
}

void BulletPhysicMatrixToGeometry::run()
{
    btTransform *vTransformation=new btTransform();
    for(int i=m_ThreadId,j=m_BuPhObList->size();i<j;i+=m_Threads)
    {
        if((*m_BuPhObList)[i]->getRigidBody()->isActive())
        {
            (*m_BuPhObList)[i]->getMotionState()->getWorldTransform(*vTransformation);
            btScalar vMatrix[16];
            vTransformation->getOpenGLMatrix(vMatrix);
            QMatrix4x4 v4x4Matrix((float)vMatrix[0],(float)vMatrix[4],(float)vMatrix[8],(float)vMatrix[12]
                    ,(float)vMatrix[1],(float)vMatrix[5],(float)vMatrix[9],(float)vMatrix[13]
                    ,(float)vMatrix[2],(float)vMatrix[6],(float)vMatrix[10],(float)vMatrix[14]
                    ,(float)vMatrix[3],(float)vMatrix[7],(float)vMatrix[11],(float)vMatrix[15]);
            (*m_BuPhObList)[i]->setGeometryModelMatrix(&v4x4Matrix);
        }
    }
    delete vTransformation;
}
