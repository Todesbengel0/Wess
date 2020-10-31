#ifndef BULLETPHYSICENGINE_H
#define BULLETPHYSICENGINE_H

#include "../physicengine.h"

#include "btBulletDynamicsCommon.h"
#include <QMutex>
#include <QList>
#include <QMatrix4x4>
#include <QThreadPool>
#include "bulletdiscretedynamicsworld.h"

 //Comment it out to use single threading
#if (defined(USE_LIBSPE2) || defined(_WIN32) || defined(USE_PTHREADS))
//#define USE_BT_MULTITHREADING 1
#define __BT_SKIP_UINT64_H 1
//#define USE_PARALLEL_SOLVER 1 //Parallel Solver is not as fast as the sequential Solver
#define USE_PARALLEL_DISPATCHER 1

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"

#ifdef USE_PARALLEL_DISPATCHER
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#include "BulletMultiThreaded/PlatformDefinitions.h"

#ifdef USE_LIBSPE2
#include "BulletMultiThreaded/SpuLibspe2Support.h"
#elif defined (_WIN32)
#include "BulletMultiThreaded/Win32ThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"

#elif defined (USE_PTHREADS)

#include "BulletMultiThreaded/PosixThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"

#else
//other platforms run the parallel code sequentially (until pthread support or other parallel implementation is added)

#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"
#endif //USE_LIBSPE2
#ifdef USE_PARALLEL_SOLVER
#include "BulletMultiThreaded/btParallelConstraintSolver.h"
#include "BulletMultiThreaded/SequentialThreadSupport.h"

#endif //USE_PARALLEL_SOLVER
#endif //USE_PARALLEL_DISPATCHER
#include "LinearMath/btQuickprof.h"
#endif //defined(USE_LIBSPE2) || defined(_WIN32) || defined(USE_PTHREADS)

class PhysicEngineManager;
class BulletPhysicMatrixToGeometry;
class BulletPhysicObject;
class BulletDebugDrawer;
class IGeometryAccess;

class BulletPhysicEngine : public PhysicEngine
{
    friend class PhysicEngineManager;
    friend class BulletPhysicObject;
protected:
    BulletPhysicEngine();
    ~BulletPhysicEngine();
    BulletPhysicEngine(const BulletPhysicEngine &);
    BulletPhysicEngine& operator=(const BulletPhysicEngine &);
    void addAsBoxAABB(PhysicObject *a_PhysicObject);
    void addAsBVHTriangleMesh(PhysicObject *a_PhysicObject);
    void addAsCone(PhysicObject *a_PhysicObject);//TODO Continious Collision Detection
    void addAsConvexHull(PhysicObject *a_PhysicObject);//TODO Continious Collision Detection
    void addAsCovarianceBoxFromPoints(PhysicObject *a_PhysicObject); //TODO Continious Collision Detection
    void addAsCustomBox(PhysicObject *a_PhysicObject);
    void addAsCustomSphere(PhysicObject *a_PhysicObject);
    void addAsMultisphere(PhysicObject *a_PhysicObject);
    void addAsSphereAABB(PhysicObject *a_PhysicObject);
    void addAsWelzlSphere(PhysicObject *a_PhysicObject);
public:
    void addHingeConstraint(PhysicObject* a_PhysicObjectA,PhysicObject* a_PhysicObjectB,
                                    QVector3D a_PivotInA,QVector3D a_PivotInB,
                                    QVector3D a_AxisInA,QVector3D a_AxisInB,
                                    float a_LowLimit,float a_HighLimit,float a_Softness=0.9f,float a_BiasFactor=0.3f,float a_RelaxationFactor=1.f);
    void addPhysicObject(PhysicObject *a_PhysicObject);
    void addPointToPointConstraint(PhysicObject *a_PhysicObjectA, QVector3D a_PivotInsideA, PhysicObject *a_PhysicObjectB, QVector3D a_PivotInsideB);
protected:
    void copyMatrixToGeometry();
    bool createAndRegisterRigidBody(BulletPhysicObject* a_BulletPhysicObject, btCollisionShape *a_CollisionShape, btTransform &a_HullTranslation);
    btBoxShape *createCollShapeBox(const QVector<QVector4D> &a_Vertices, btTransform &a_Transformation);
public:
    DynamicCharacter* createNewDynamicCharacter(IGeometryAccess *a_Geometry);
    DynamicCharacterWithCam* createNewDynamicCharacterWithCam(IGeometryAccess *a_Geometry);
    PhysicObject* createNewPhysicObject(IGeometryAccess *a_Geometry);
protected:
#ifdef USE_PARALLEL_SOLVER
    btThreadSupportInterface *createSolverThreadSupport(int a_MaxNumThreads);
#endif //USE_PARALLEL_SOLVER
public:
    void debugDraw();

    static QMatrix4x4 getTransformation(btTransform &a_BtMatrix);
    static btTransform getTransformation(QMatrix4x4 &a_QtMatrix);
    btCollisionWorld* getCollisionWorld();
    QString getDebugMessage();

protected:
    btVector3 QVector3DTobtVector3(const QVector3D a_Vector);
    QVector3D btVector3ToQVector3D(const btVector3& a_Vector);

public:
    void pauseSimulation();

public:
    PhysicObject* rayTestClosestBody(QVector3D a_From, QVector3D a_To);
    QList<PhysicObject*> rayTestAllBodys(QVector3D a_From, QVector3D a_To);
protected:
    void deleteConstraints(PhysicObject *a_PhysicObjectA, PhysicObject *a_PhysicObjectB);
    void removePhysicObject(PhysicObject *a_PhysicObject);
public:
    void resumeSimulation();

    void setGravityForNewObjects(QVector3D a_Gravity);
    void setGravityForAllObjects(QVector3D a_Gravity);
    void setFrequency(float a_Frequency);

    void tick(int timeElapsedMS);
public:
    void updateMatrix();
protected:
    btDefaultCollisionConfiguration *m_CollisionConfiguration;
    btCollisionDispatcher *m_CollisionDispatcher;
    btSequentialImpulseConstraintSolver *m_ConstraintSolver;

    BulletDebugDrawer* m_DebugDrawer;
    bool m_DebugDrawerInitialized;
    unsigned long long int m_DrawingThreadID;
    QList<BulletPhysicObject*> m_DynamicPhysicObjects;
    BulletDiscreteDynamicsWorld *m_DynamicsWorld;

    float m_Frequency;

    bool m_IsEmpty;

    QList<BulletPhysicObject*> m_KinematicPhysicObjects;

    QList<BulletPhysicMatrixToGeometry*> m_MatrixCopyThreads;

    int m_NumberOfThreads;

    QMutex m_Mutex;

    btBroadphaseInterface *m_OverlappingPairCache;

    bool m_SimulationRunning;
    QList<BulletPhysicObject*> m_StaticPhysicObjects;

    QThreadPool* m_ThreadPool;
#ifdef USE_BT_MULTITHREADING
#ifdef USE_PARALLEL_DISPATCHER
    btThreadSupportInterface *m_ThreadSupportCollision;
#endif //USE_PARALLEL_DISPATCHER
#ifdef USE_PARALLEL_SOLVER
    btThreadSupportInterface *m_ThreadSupportSolver;
#endif //USE_PARALLEL_SOLVER
#endif //USE_BT_MULTITHREADING
    bool m_UpdateMatrix;
};
#endif // BULLETPHYSICENGINE_H
