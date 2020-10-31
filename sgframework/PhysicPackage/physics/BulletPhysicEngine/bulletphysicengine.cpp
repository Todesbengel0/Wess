#include "bulletphysicengine.h"
#include "bulletphysicobject.h"
#include "bulletphysicmatrixtogeometry.h"
#include "bulletcollisiondispatcher.h"
#include "bulletspucollisiondispatcher.h"
#include "LinearMath/btGeometryUtil.h"
#include "bulletdebugdrawer.h"
#include "bulletdynamiccharacter.h"
#include "bulletdynamiccharacterwithcam.h"
#include "../boundingsphere.h"
#include "../boundingbox.h"
#include "../igeometryaccess.h"

BulletPhysicEngine::BulletPhysicEngine():PhysicEngine()
{
    const btScalar v_Gravity=-9.81f;//-58.86f
    m_Frequency=1.f/300.f;
    m_DrawingThreadID=0;
    m_DebugDrawerInitialized=false;
    m_UpdateMatrix=false;
    m_IsEmpty=true;
    m_SimulationRunning=true;
    m_NumberOfThreads=QThread::idealThreadCount();
    qDebug()<<"BulletPhysicEngine::BulletPhysicEngine(): "<<m_NumberOfThreads<<" Threads used";
    if(m_NumberOfThreads<1)
    {
        m_NumberOfThreads=1;
    }
    //qDebug()<<"BulletPhysicEngine::BulletPhysicEngine(): Phyisc Engine runs on "<<mNumberOfThreads;
#ifndef USE_BT_MULTITHREADING
    m_CollisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_CollisionDispatcher = new	BulletCollisionDispatcher(m_CollisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_OverlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_ConstraintSolver = new btSequentialImpulseConstraintSolver;

    m_DynamicsWorld = new BulletDiscreteDynamicsWorld(this,m_CollisionDispatcher,m_OverlappingPairCache,
                                                 m_ConstraintSolver,m_CollisionConfiguration);
    m_DynamicsWorld->setGravity(btVector3((btScalar)0.,(btScalar)v_Gravity,(btScalar)0.));
#else
#ifdef USE_PARALLEL_DISPATCHER
#ifdef USE_PARALLEL_SOLVER
    m_ThreadSupportSolver = 0;
#endif //USE_PARALLEL_SOLVER
    m_ThreadSupportCollision = 0;
#endif //USE_PARALLEL_DISPATCHER
    m_CollisionDispatcher=0;
    btDefaultCollisionConstructionInfo v_Cci;
    v_Cci.m_defaultMaxPersistentManifoldPoolSize = /*32768*/100000;
    this->m_CollisionConfiguration = new btDefaultCollisionConfiguration(v_Cci);

#ifdef USE_PARALLEL_DISPATCHER
    int v_MaxNumOutstandingTasks = m_NumberOfThreads;

#ifdef USE_WIN32_THREADING
    m_ThreadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
                                                         "collision",
                                                         processCollisionTask,
                                                         createCollisionLocalStoreMemory,
                                                         v_MaxNumOutstandingTasks));
#else

#ifdef USE_LIBSPE2

    spe_program_handle_t * program_handle;
#ifndef USE_CESOF
    program_handle = spe_image_open ("./spuCollision.elf");
    if (program_handle == 0)
    {
        perror( "SPU OPEN IMAGE ERROR\n");
    }
    else
    {
        printf( "IMAGE OPENED\n");
    }
#else
    extern spe_program_handle_t spu_program;
    program_handle = &spu_program;
#endif
    SpuLibspe2Support* threadSupportCollision  = new SpuLibspe2Support( program_handle, v_MaxNumOutstandingTasks);
#elif defined (USE_PTHREADS)
    PosixThreadSupport::ThreadConstructionInfo constructionInfo("collision",
                                                                processCollisionTask,
                                                                createCollisionLocalStoreMemory,
                                                                v_MaxNumOutstandingTasks);
    m_ThreadSupportCollision = new PosixThreadSupport(constructionInfo);
#else

    SequentialThreadSupport::SequentialThreadConstructionInfo colCI("collision",processCollisionTask,createCollisionLocalStoreMemory);
    SequentialThreadSupport* mThreadSupportCollision = new SequentialThreadSupport(colCI);

#endif //USE_LIBSPE2

    ///Playstation 3 SPU (SPURS)  version is available through PS3 Devnet
    /// For Unix/Mac someone could implement a pthreads version of btThreadSupportInterface?
    ///you can hook it up to your custom task scheduler by deriving from btThreadSupportInterface
#endif
    m_CollisionDispatcher = new	BulletSpuCollisionDispatcher(m_ThreadSupportCollision,v_MaxNumOutstandingTasks,m_CollisionConfiguration);
#else
    m_CollisionDispatcher = new	BulletCollisionDispatcher(m_CollisionConfiguration);
#endif //USE_PARALLEL_DISPATCHER
//        btVector3 worldAabbMin(-1000,-1000,-1000);
//        btVector3 worldAabbMax(1000,1000,1000);
//        const int maxProxies = 32766;
//        mOverlappingPairCache = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    m_OverlappingPairCache = new btDbvtBroadphase();

#ifdef USE_PARALLEL_SOLVER
    m_ThreadSupportSolver = createSolverThreadSupport(v_MaxNumOutstandingTasks);
    m_ConstraintSolver = new btParallelConstraintSolver(m_ThreadSupportSolver);
    //    this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
    m_CollisionDispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
#else
    m_ConstraintSolver =  new btSequentialImpulseConstraintSolver();
    //default solverMode is SOLVER_RANDMIZE_ORDER. Warmstarting seems not to improve convergence, see
    //solver->setSolverMode(0);//btSequentialImpulseConstraintSolver::SOLVER_USE_WARMSTARTING | btSequentialImpulseConstraintSolver::SOLVER_RANDMIZE_ORDER);
#endif //USE_PARALLEL_SOLVER
    m_DynamicsWorld =  new BulletDiscreteDynamicsWorld(m_CollisionDispatcher,m_OverlappingPairCache,m_ConstraintSolver,m_CollisionConfiguration);
    m_DynamicsWorld->getSimulationIslandManager()->setSplitIslands(true);
    m_DynamicsWorld->getSolverInfo().m_numIterations = 4;
    m_DynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD+SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;
    m_DynamicsWorld->getDispatchInfo().m_enableSPU = true;
    m_DynamicsWorld->setGravity(btVector3((btScalar)0.,(btScalar)v_Gravity,(btScalar)0.));
#endif //USE_BT_MULTITHREADING

    m_DebugDrawer=new BulletDebugDrawer();
    m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
    m_ThreadPool=new QThreadPool();
    m_ThreadPool->setMaxThreadCount(this->m_NumberOfThreads);
    m_ThreadPool->setExpiryTimeout(-1);
    for(int i=0;i<m_NumberOfThreads;i++)
    {
        m_MatrixCopyThreads.append(new BulletPhysicMatrixToGeometry(&m_DynamicPhysicObjects,i,this->m_NumberOfThreads));
    }
}

BulletPhysicEngine::~BulletPhysicEngine()
{
    {
        int v_SizeOfList=m_DynamicPhysicObjects.size();
        BulletPhysicObject** v_PhysicObjectsToDelete=new BulletPhysicObject* [v_SizeOfList];
        for (int i=0;i<v_SizeOfList;i++)
        {
            v_PhysicObjectsToDelete[i]=m_DynamicPhysicObjects[i];
        }
        for (int i=0;i<v_SizeOfList;i++)
        {
            delete v_PhysicObjectsToDelete[i];
        }
        m_DynamicPhysicObjects.clear();
        delete [] v_PhysicObjectsToDelete;
    }

    {
        int v_SizeOfList=m_StaticPhysicObjects.size();
        BulletPhysicObject** v_PhysicObjectsToDelete=new BulletPhysicObject* [v_SizeOfList];
        for (int i=0;i<v_SizeOfList;i++)
        {
            v_PhysicObjectsToDelete[i]=m_StaticPhysicObjects[i];
        }
        for (int i=0;i<v_SizeOfList;i++)
        {
            delete v_PhysicObjectsToDelete[i];
        }
        m_StaticPhysicObjects.clear();
        delete [] v_PhysicObjectsToDelete;
    }

    {
        int v_SizeOfList=m_KinematicPhysicObjects.size();
        BulletPhysicObject** v_PhysicObjectsToDelete=new BulletPhysicObject* [v_SizeOfList];
        for (int i=0;i<v_SizeOfList;i++)
        {
            v_PhysicObjectsToDelete[i]=m_KinematicPhysicObjects[i];
        }
        for (int i=0;i<v_SizeOfList;i++)
        {
            delete v_PhysicObjectsToDelete[i];
        }
        m_KinematicPhysicObjects.clear();
        delete [] v_PhysicObjectsToDelete;
    }

    for (int i=0;i<m_MatrixCopyThreads.size();i++)
    {
        delete m_MatrixCopyThreads[i];
    }
    m_MatrixCopyThreads.clear();

    delete m_CollisionConfiguration;
    delete m_CollisionDispatcher;
    delete m_OverlappingPairCache;
    delete m_ConstraintSolver;
    delete m_DynamicsWorld;
    delete m_ThreadPool;
    delete m_DebugDrawer;
}

void BulletPhysicEngine::addAsBoxAABB(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;

    //Get Vertex List from Geometry
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();

    //Abort if No Vertex Data is Given
    if(v_VerticeList.size()<3)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsBox(Geometry *aGeometry): Abort, No vertex Data Given";
        return;
    }
    //Create Collision Shape from Points

    //Set Hull Transformation for Correct passing the Shape in the Geometry
    btTransform v_HullTranslation;

    //Set Identity Matrix
    v_HullTranslation.setIdentity();

    //Generate a Pointer to a Box Shape
    btBoxShape *v_BoxShape=createCollShapeBox(v_VerticeList,v_HullTranslation);
    if(!v_BoxShape)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsBox(Geometry *aGeometry): No Collision Hull generated";
        return;
    }

    //Create and Register RigidBody
    createAndRegisterRigidBody(v_BulletPhysicObject,v_BoxShape,v_HullTranslation);

    //Calculate Sphere for Continious Collision Detection
    if(v_BulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        //Get the Shortest Value of the half extends which is the radius for the CCD Sphere
        btVector3 vHalfExtends=v_BoxShape->getHalfExtentsWithMargin();
        btScalar v_ShortestExtendValue=vHalfExtends.x();
        if(v_ShortestExtendValue>vHalfExtends.y())
        {
            v_ShortestExtendValue=vHalfExtends.y();
        }
        if(v_ShortestExtendValue>vHalfExtends.z())
        {
            v_ShortestExtendValue=vHalfExtends.z();
        }

        v_BulletPhysicObject->m_RigidBody->setCcdMotionThreshold(v_ShortestExtendValue);
        v_BulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius(v_ShortestExtendValue);
    }
}

void BulletPhysicEngine::addAsBVHTriangleMesh(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;

    //Get midpoint transformation for correct Position
    QMatrix4x4 v_MidpointTransformation=v_BulletPhysicObject->getConstructionInfo()->getMidpointTransformation();
    btTransform v_HullTranslation=getTransformation(v_MidpointTransformation);

    //Look if vertex data is correct
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();
    int v_ListSize=v_VerticeList.size();
    if(v_ListSize%3 || v_ListSize<2)
    {
        qDebug()<<"BulletPhysicEngine::addAsBVHTriangleMesh(PhysicObject *a_PhysicObject): Failed";
        return;
    }

    //Create new trianglemesh
    btTriangleMesh* v_TriangleMesh=new btTriangleMesh();

    for(int i=2;i<v_ListSize;i+=3)
    {
        btVector3 v_VerticeA=btVector3((btScalar)v_VerticeList[i-2].x(),(btScalar)v_VerticeList[i-2].y(),(btScalar)v_VerticeList[i-2].z());
        btVector3 v_VerticeB=btVector3((btScalar)v_VerticeList[i-1].x(),(btScalar)v_VerticeList[i-1].y(),(btScalar)v_VerticeList[i-1].z());
        btVector3 v_VerticeC=btVector3((btScalar)v_VerticeList[i].x(),(btScalar)v_VerticeList[i].y(),(btScalar)v_VerticeList[i].z());
        v_TriangleMesh->addTriangle(v_VerticeA,v_VerticeB,v_VerticeC);
    }
    btBvhTriangleMeshShape *v_TriangleMeshShape= new btBvhTriangleMeshShape(v_TriangleMesh,true,true);

    createAndRegisterRigidBody(v_BulletPhysicObject,v_TriangleMeshShape,v_HullTranslation);
}

void BulletPhysicEngine::addAsCone(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;

    QMatrix4x4 v_MidpointTransformation=v_BulletPhysicObject->getConstructionInfo()->getMidpointTransformation();
    v_MidpointTransformation.translate(0.f,0.04f,0.f);
    btTransform v_HullTranslation=getTransformation(v_MidpointTransformation);

    PhysicObjectConstructionInfo* v_ConstrInfo=v_BulletPhysicObject->getConstructionInfo();
    float v_Radius=v_ConstrInfo->getConeRadius();
    float v_Heigth=v_ConstrInfo->getConeHeight();

    btConeShape* v_ConeShape=new btConeShape(v_Radius-0.04,v_Heigth-0.08);

    createAndRegisterRigidBody(v_BulletPhysicObject,v_ConeShape,v_HullTranslation);
}

void BulletPhysicEngine::addAsConvexHull(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();

    QMatrix4x4 v_MidpointTransformation=v_BulletPhysicObject->getConstructionInfo()->getMidpointTransformation();
    btTransform v_HullTranslation=getTransformation(v_MidpointTransformation);

    btAlignedObjectArray<btVector3> v_ScaledVertices;
    {
        btAlignedObjectArray<btVector3> v_PlaneEquations;
        btAlignedObjectArray<btVector3> v_UnscaledVertices;

        for(int i=0,j=v_VerticeList.size();i<j;i++)
        {
            v_UnscaledVertices.push_back(btVector3(v_VerticeList[i].x(),v_VerticeList[i].y(),v_VerticeList[i].z()));
        }
        btGeometryUtil::getPlaneEquationsFromVertices(v_UnscaledVertices,v_PlaneEquations);

        btAlignedObjectArray<btVector3> v_ShiftedPlaneEquation;
        for(int i=0,j=v_PlaneEquations.size();i<j;i++)
        {
            btVector3 v_PlaneTemp=v_PlaneEquations[i];
            v_PlaneTemp[3]+=0.04f;
            v_ShiftedPlaneEquation.push_back(v_PlaneTemp);
        }
        btGeometryUtil::getVerticesFromPlaneEquations(v_ShiftedPlaneEquation,v_ScaledVertices);
    }

    btConvexHullShape* v_ConvexHullShape= new btConvexHullShape(&(v_ScaledVertices[0].getX()),v_ScaledVertices.size());

    v_ConvexHullShape->setMargin(0.04f);

    createAndRegisterRigidBody(v_BulletPhysicObject,v_ConvexHullShape,v_HullTranslation);
}

void BulletPhysicEngine::addAsCovarianceBoxFromPoints(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();
    QVector3D v_BoxHalfExtends;
    QMatrix4x4 v_Transformation;
    BoundingBox::getCovarianceOptimizedBoxFromPoints(v_VerticeList,v_BoxHalfExtends,v_Transformation);

    btTransform v_HullTranslation=getTransformation(v_Transformation);
    btBoxShape *v_BoxShape=new btBoxShape(btVector3((btScalar)v_BoxHalfExtends.x(),(btScalar)v_BoxHalfExtends.y(),(btScalar)v_BoxHalfExtends.z()));

    createAndRegisterRigidBody(v_BulletPhysicObject,v_BoxShape,v_HullTranslation);
}

void BulletPhysicEngine::addAsCustomBox(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* vBulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    QMatrix4x4 v_MidpointTransformation=vBulletPhysicObject->getConstructionInfo()->getMidpointTransformation();
    btTransform vHullTranslation=getTransformation(v_MidpointTransformation);
    QVector3D v_HalfExtends=vBulletPhysicObject->getConstructionInfo()->getBoxHalfExtends();
    btBoxShape *vBoxShape=new btBoxShape(btVector3((btScalar)(v_HalfExtends.x()),(btScalar)(v_HalfExtends.y()),(btScalar)(v_HalfExtends.z())));

    //Create and Register RigidBody
    createAndRegisterRigidBody(vBulletPhysicObject,vBoxShape,vHullTranslation);
    //Calculate Sphere for Continious Collision Detection
    if(vBulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        //Get the Shortest Value of the half extends which is the radius for the CCD Sphere
        btVector3 vHalfExtends=vBoxShape->getHalfExtentsWithMargin();
        btScalar vShortestExtendValue=vHalfExtends.x();
        if(vShortestExtendValue>vHalfExtends.y())
        {
            vShortestExtendValue=vHalfExtends.y();
        }
        if(vShortestExtendValue>vHalfExtends.z())
        {
            vShortestExtendValue=vHalfExtends.z();
        }
        vBulletPhysicObject->m_RigidBody->setCcdMotionThreshold(2.f*vShortestExtendValue);
        vBulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius(vShortestExtendValue);
    }
}

void BulletPhysicEngine::addAsCustomSphere(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* vBulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    //Create Collision Shape from Radius
    //Set Hull Transformation for Correct passing the Shape in the Geometry
    QMatrix4x4 v_MidpointTransformation=a_PhysicObject->getConstructionInfo()->getMidpointTransformation();
    btTransform vHullTranslation=getTransformation(v_MidpointTransformation);;

    //Create Collisionshape for sphere
    btSphereShape *vSphereShape=0;
    vSphereShape=new btSphereShape((btScalar)(a_PhysicObject->getConstructionInfo()->getSphereRadius()));
    //If no Collision Shape is generated Abort
    if(!vSphereShape)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsSphere(Geometry *aGeometry): No Collision Hull generated";
        return;
    }
    //Create and Register RigidBody
    createAndRegisterRigidBody(vBulletPhysicObject,vSphereShape,vHullTranslation);
    //Calculate Sphere for Continious Collision Detection
    if(vBulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        vBulletPhysicObject->m_RigidBody->setCcdMotionThreshold(2.f*(btScalar)(a_PhysicObject->getConstructionInfo()->getSphereRadius()));
        vBulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius((btScalar)(a_PhysicObject->getConstructionInfo()->getSphereRadius()));
    }
}

void BulletPhysicEngine::addAsMultisphere(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    QMatrix4x4 v_MidpointTransformation=a_PhysicObject->getConstructionInfo()->getMidpointTransformation();
    btTransform  v_HullTranslation=getTransformation(v_MidpointTransformation);
    btMultiSphereShape *v_MultisphereShape=0;
    btVector3* v_Positions=0;
    btScalar* v_Radius=0;
    int v_NumberOfSpheres;
    QList<SphereConstructionInfo> v_MultiSpherConstrInf=a_PhysicObject->getConstructionInfo()->getMultiSphereConstructionInfo();
    v_NumberOfSpheres=v_MultiSpherConstrInf.size();
    v_Positions=new btVector3 [v_NumberOfSpheres]();
    v_Radius=new btScalar [v_NumberOfSpheres]();
    for(int i=0;i<v_NumberOfSpheres;i++)
    {
        QMatrix4x4 v_MidPoiTrans=v_MultiSpherConstrInf[i].getMidpointTransformation();
        v_Positions[i]=btVector3((btScalar)(v_MidPoiTrans.data()[12]),(btScalar)(v_MidPoiTrans.data()[13]),(btScalar)(v_MidPoiTrans.data()[14]));
        v_Radius[i]=(btScalar)v_MultiSpherConstrInf[i].getSphereRadius();
    }
    v_MultisphereShape=new btMultiSphereShape(v_Positions,v_Radius,v_NumberOfSpheres);
    createAndRegisterRigidBody(v_BulletPhysicObject,v_MultisphereShape,v_HullTranslation);
    delete[] v_Positions;
    delete[] v_Radius;
    if(v_BulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        btScalar v_Rad2=(btScalar)(v_MultiSpherConstrInf[0].getSphereRadius());
        for(int i=1;i<v_NumberOfSpheres;i++)
        {
            if(v_Rad2>(btScalar)(v_MultiSpherConstrInf[i].getSphereRadius()))
            {
                v_Rad2=(btScalar)(v_MultiSpherConstrInf[i].getSphereRadius());
            }
        }
        v_BulletPhysicObject->m_RigidBody->setCcdMotionThreshold(v_Rad2);
        v_BulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius(v_Rad2);
    }
}

void BulletPhysicEngine::addAsSphereAABB(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)(a_PhysicObject);
    //Get Vertex List from Geometry
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();
    //Abort if No Vertex Data is Given
    if(v_VerticeList.size()<3 || v_VerticeList.size()%3!=0)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsSphere(Geometry *aGeometry): Abort, No vertex Data Given";
        return;
    }
    //Create Collision Shape from Points
    //Set Hull Transformation for Correct passing the Shape in the Geometry
    btTransform v_HullTranslation;
    //Set Identity Matrix
    v_HullTranslation.setIdentity();
    //Generate a Pointer to a Box Shape
    btBoxShape *v_BoxShape=0;
    //create the box Shape out of the Vertex Data
    v_BoxShape=createCollShapeBox(v_VerticeList,v_HullTranslation);
    //If no BoxShape was created return
    if(!v_BoxShape)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsBox(Geometry *aGeometry): No Box Shape generated";
        return;
    }
    //Get the Half extends of the box
    //to get the Vector Length of the half extends which is the radius for the sphere
    btVector3 v_HalfExtends=v_BoxShape->getHalfExtentsWithMargin();
    btScalar v_Radius=v_HalfExtends.length();
    qDebug()<<v_Radius<<v_HalfExtends.x()<<v_HalfExtends.y()<<v_HalfExtends.z();
    //Free Space of vBoxShape
    delete v_BoxShape;
    v_BoxShape=0;

    //Create Collisionshape for sphere
    btSphereShape *v_SphereShape=0;
    v_SphereShape=new btSphereShape(v_Radius);
    //If no Collision Shape is generated Abort
    if(!v_SphereShape)
    {
        qDebug()<<"[ABORT]BulletPhysicEngine::addAsSphere(Geometry *aGeometry): No Collision Hull generated";
        return;
    }
    //Create and Register RigidBody
    createAndRegisterRigidBody(v_BulletPhysicObject,v_SphereShape,v_HullTranslation);
    //Calculate Sphere for Continious Collision Detection
    if(v_BulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        v_BulletPhysicObject->m_RigidBody->setCcdMotionThreshold(v_Radius);
        v_BulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius(v_Radius);
    }
}

void BulletPhysicEngine::addAsWelzlSphere(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=(BulletPhysicObject*)a_PhysicObject;
    const QVector<QVector4D> v_VerticeList=a_PhysicObject->getGeometry()->getTriangleList();
    BoundingSphere v_BoundingSphere=BoundingSphere::getWelzlSphere(v_VerticeList);

    QVector3D v_Temp=v_BoundingSphere.getCenter();
    btVector3 v_Center=btVector3(v_Temp.x(),v_Temp.y(),v_Temp.z());
    btTransform v_HullTranslation;
    v_HullTranslation.setIdentity();
    v_HullTranslation.setOrigin(v_Center);

    float v_Radius=v_BoundingSphere.getRadius();

    btSphereShape *v_SphereShape=new btSphereShape(v_Radius);
    createAndRegisterRigidBody(v_BulletPhysicObject,v_SphereShape,v_HullTranslation);
    if(v_BulletPhysicObject->getConstructionInfo()->getCcdActivation())
    {
        v_BulletPhysicObject->m_RigidBody->setCcdMotionThreshold(2.f*v_Radius);
        v_BulletPhysicObject->m_RigidBody->setCcdSweptSphereRadius(v_Radius);
    }
}

void BulletPhysicEngine::addHingeConstraint(PhysicObject *a_PhysicObjectA, PhysicObject *a_PhysicObjectB, QVector3D a_PivotInA, QVector3D a_PivotInB, QVector3D a_AxisInA, QVector3D a_AxisInB, float a_LowLimit, float a_HighLimit, float a_Softness, float a_BiasFactor, float a_RelaxationFactor)
{
    btRigidBody *v_RigidBodyA=((BulletPhysicObject*)a_PhysicObjectA)->m_RigidBody;
    btRigidBody *v_RigidBodyB=((BulletPhysicObject*)a_PhysicObjectB)->m_RigidBody;
    btVector3 v_PivotInA=btVector3((btScalar)(a_PivotInA.x()),(btScalar)(a_PivotInA.y()),(btScalar)(a_PivotInA.z()));
    btVector3 v_PivotInB=btVector3((btScalar)(a_PivotInB.x()),(btScalar)(a_PivotInB.y()),(btScalar)(a_PivotInB.z()));
    btVector3 v_AxisInA=btVector3((btScalar)(a_AxisInA.x()),(btScalar)(a_AxisInA.y()),(btScalar)(a_AxisInA.z()));
    btVector3 v_AxisInB=btVector3((btScalar)(a_AxisInB.x()),(btScalar)(a_AxisInB.y()),(btScalar)(a_AxisInB.z()));

    btHingeConstraint* v_HingeConstraint=new btHingeConstraint(*v_RigidBodyA,*v_RigidBodyB,v_PivotInA,v_PivotInB,v_AxisInA,v_AxisInB);

    ConstraintPair v_ConstraintPair;
    v_ConstraintPair.m_Constraint=v_HingeConstraint;
    v_ConstraintPair.m_PhysicObject=a_PhysicObjectB;
    ((BulletPhysicObject*)a_PhysicObjectA)->addConstraintPair(v_ConstraintPair);
    v_ConstraintPair.m_PhysicObject=a_PhysicObjectA;
    ((BulletPhysicObject*)a_PhysicObjectB)->addConstraintPair(v_ConstraintPair);

    m_DynamicsWorld->addConstraint(v_HingeConstraint,true);

    v_HingeConstraint->setLimit((btScalar)a_LowLimit,(btScalar)a_HighLimit,(btScalar)a_Softness,(btScalar)a_BiasFactor,(btScalar)a_RelaxationFactor);
}

void BulletPhysicEngine::addPhysicObject(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=dynamic_cast<BulletPhysicObject*>(a_PhysicObject);
    if(v_BulletPhysicObject)
    {
        if(v_BulletPhysicObject->m_RigidBody && v_BulletPhysicObject->m_PhysicEngine==this)
        {
            unsigned int vPhysicState=a_PhysicObject->getPhysicState();
            switch (vPhysicState)
            {
            case PhysicState::Dynamic:
            {
                m_DynamicPhysicObjects.append(v_BulletPhysicObject);
                break;
            }
            case PhysicState::Static:
            {
                m_StaticPhysicObjects.append(v_BulletPhysicObject);
                break;
            }
            case PhysicState::Kinematic:
            {
                m_KinematicPhysicObjects.append(v_BulletPhysicObject);
                break;
            }
            default:
            {
                qDebug()<<"[WARNING]BulletPhysicEngine::addPhysicObject(PhysicObject *aPhysicObject): No Valid PhysicState in PhysicObject";
                return;
            }
            }
            this->m_DynamicsWorld->addRigidBody(((BulletPhysicObject*)(a_PhysicObject))->m_RigidBody);
            for(int i=0,j=v_BulletPhysicObject->m_ConstraintPairList.size();i<j;i++)
            {
                m_DynamicsWorld->addConstraint(v_BulletPhysicObject->m_ConstraintPairList[i].m_Constraint,true);
            }
        }
        else
        {
            qDebug()<<"[WARNING]BulletPhysicEngine::addPhysicObject(PhysicObject *aPhysicObject): No RigidBody Constructed in PhysicObject";
            return;
        }
    }
    else
    {
        qDebug()<<"[WARNING]BulletPhysicEngine::addPhysicObject(PhysicObject *aPhysicObject): No RigidBody Constructed in PhysicObject";
        return;
    }
}

void BulletPhysicEngine::addPointToPointConstraint(PhysicObject *a_PhysicObjectA, QVector3D a_PivotInsideA, PhysicObject *a_PhysicObjectB, QVector3D a_PivotInsideB)
{
    btRigidBody *v_RigidBody1=((BulletPhysicObject*)a_PhysicObjectA)->m_RigidBody;
    btRigidBody *v_RigidBody2=((BulletPhysicObject*)a_PhysicObjectB)->m_RigidBody;
    btPoint2PointConstraint *v_Constr=new btPoint2PointConstraint(*v_RigidBody1,
                                                                 *v_RigidBody2,
                                                                 btVector3((btScalar)(a_PivotInsideA.x()),(btScalar)(a_PivotInsideA.y()),(btScalar)(a_PivotInsideA.z())),
                                                                 btVector3((btScalar)(a_PivotInsideB.x()),(btScalar)(a_PivotInsideB.y()),(btScalar)(a_PivotInsideB.z())));
    ConstraintPair v_ConstraintPairGeo;
    v_ConstraintPairGeo.m_Constraint=v_Constr;
    v_ConstraintPairGeo.m_PhysicObject=a_PhysicObjectB;
    ((BulletPhysicObject*)a_PhysicObjectA)->addConstraintPair(v_ConstraintPairGeo);
    v_ConstraintPairGeo.m_PhysicObject=a_PhysicObjectA;
    ((BulletPhysicObject*)a_PhysicObjectB)->addConstraintPair(v_ConstraintPairGeo);

    m_DynamicsWorld->addConstraint(v_Constr,true);
}

void BulletPhysicEngine::copyMatrixToGeometry()
{
    for(int i=0;i<this->m_NumberOfThreads;i++)
    {
        m_ThreadPool->start(m_MatrixCopyThreads.at(i));
    }
    m_ThreadPool->waitForDone(-1);
}

bool BulletPhysicEngine::createAndRegisterRigidBody(BulletPhysicObject *a_BulletPhysicObject, btCollisionShape* a_CollisionShape, btTransform &a_HullTranslation)
{
    //Calculate Local Inertia if mass is not 0.0
    //Massepunkt relativ vom mittelpunkt der verschobenen hülle also von a_HullTranslation aus
    btVector3 v_LocalInertiaPoint((btScalar)a_BulletPhysicObject->getConstructionInfo()->getLocalInertiaPoint().x(),
                                 (btScalar)a_BulletPhysicObject->getConstructionInfo()->getLocalInertiaPoint().y(),
                                 (btScalar)a_BulletPhysicObject->getConstructionInfo()->getLocalInertiaPoint().z());

    //Get Center Of Mass Transformation
    //QtMatrizen holen um den absoluten masseverschiebungspunkt zu berechnen relativ zum objektmittelpunkt und nicht kollisionshüllenmittelpunkt
   /* QMatrix4x4 v_MidPointTrans=a_BulletPhysicObject->getConstructionInfo()->getMidpointTransformation();
    QVector3D v_CenterOfMassTranslation=a_BulletPhysicObject->getConstructionInfo()->getLocalInertiaPoint();
    v_CenterOfMassTranslation=v_MidPointTrans*v_CenterOfMassTranslation;//<---Massepunkt relativ vom Objektmittelpunkt
    btVector3 v_BulletCenterOfMassTranslation=btVector3((btScalar)(v_CenterOfMassTranslation.x()),
                                                        (btScalar)(v_CenterOfMassTranslation.y()),
                                                        (btScalar)(v_CenterOfMassTranslation.z()));//<---Massepunkt relativ vom Objektmittelpunkt, für bullet

    btTransform v_CenterOfMassTransformation=a_HullTranslation;
    v_CenterOfMassTransformation.setOrigin(v_CenterOfMassTransformation.getOrigin()+v_BulletCenterOfMassTranslation);
    QMatrix4x4 v_QtMatrix=this->getTransformation(v_CenterOfMassTransformation);
    a_BulletPhysicObject->setCenterOfMassTransformation(v_QtMatrix);*/

    QMatrix4x4 centerOfMassTransformationQt;
    centerOfMassTransformationQt.translate(v_LocalInertiaPoint.x(),v_LocalInertiaPoint.y(),v_LocalInertiaPoint.z());
    a_BulletPhysicObject->setCenterOfMassTransformation(centerOfMassTransformationQt);

    QMatrix4x4 hullTransformationQt=getTransformation(a_HullTranslation);
    a_BulletPhysicObject->setHullTransformation(hullTransformationQt);


    btScalar v_Mass=(btScalar)a_BulletPhysicObject->getConstructionInfo()->getMass();
    if(a_BulletPhysicObject->getPhysicState()==PhysicState::Dynamic && v_Mass >0.0f)
    {
        a_CollisionShape->calculateLocalInertia(v_Mass,v_LocalInertiaPoint);
    }
    else
    {
        v_Mass=0.0f;
    }

    //Get ModelMatrix from Geometry for Transformation in Physics World
    btTransform v_ModelMatrix;
    {
        float* v_ObjectModelMatrix=a_BulletPhysicObject->getGeometry()->getModelMatrix().data();
#ifdef BT_USE_DOUBLE_PRECISION //change btscalar and bullet-c-api
        btScalar v_Temp [16];
        for(int i=0;i<16;i++)
        {
            v_Temp[i]=(btScalar)v_ObjectModelMatrix[i];
        }
        v_ModelMatrix.setFromOpenGLMatrix(v_Temp);
#else
        v_ModelMatrix.setFromOpenGLMatrix(v_ObjectModelMatrix);
#endif
        v_ModelMatrix*=a_HullTranslation;
    }

    //Create Rigid Body
    btDefaultMotionState *v_DefaultMotionState=new btDefaultMotionState(v_ModelMatrix);
    btRigidBody::btRigidBodyConstructionInfo vRbConstrInfo(v_Mass,v_DefaultMotionState,a_CollisionShape,v_LocalInertiaPoint);
    btRigidBody *v_RigidBody=new btRigidBody(vRbConstrInfo);
    v_RigidBody->setUserPointer(a_BulletPhysicObject);
    v_RigidBody->setSleepingThresholds(0.2f,0.25f);//linear standard 0.8, angular standard 1

    //Set Friction and Restitution
    v_RigidBody->setFriction((btScalar)(a_BulletPhysicObject->getConstructionInfo()->getFriction()));
    v_RigidBody->setRestitution((btScalar)(a_BulletPhysicObject->getConstructionInfo()->getRestitution()));
    v_RigidBody->setRollingFriction((btScalar)(a_BulletPhysicObject->getConstructionInfo()->getRollingFriction()));
    if(a_BulletPhysicObject->getConstructionInfo()->getCollisionHull()==CollisionHull::SphereRadius)
    {
        v_RigidBody->setAnisotropicFriction(a_CollisionShape->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
    }

    //Add Rigid Body to Dynamic World
    m_DynamicsWorld->addRigidBody(v_RigidBody);

    //AddRigidBody to the PhysicObject
    a_BulletPhysicObject->setRigidBody(v_RigidBody);

    //Get the Motion State
    a_BulletPhysicObject->setMotionState(v_RigidBody->getMotionState());

    //Add to the List of Physic Objects in the World and Set CollisionFlagsFlags
    if(a_BulletPhysicObject->getPhysicState() == PhysicState::Static)
    {
        m_StaticPhysicObjects.append(a_BulletPhysicObject);
        v_RigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }
    else if(a_BulletPhysicObject->getPhysicState() == PhysicState::Dynamic)
    {
        m_DynamicPhysicObjects.append(a_BulletPhysicObject);
    }
    else if(a_BulletPhysicObject->getPhysicState() == PhysicState::Kinematic)
    {
        m_KinematicPhysicObjects.append(a_BulletPhysicObject);
        v_RigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }
    //If Dynamic Character Controller is used fix x and z rotation axis
    if(a_BulletPhysicObject->getPhysicType()==PhysicType::Player)
    {
        a_BulletPhysicObject->setAngularFactor(QVector3D(0.f,0.f,0.f));
        v_RigidBody->setCollisionFlags(v_RigidBody->getCollisionFlags()|btCollisionObject::CF_CHARACTER_OBJECT);
    }
    //If Set as Trigger Type deactivate Contact Response
    if(a_BulletPhysicObject->getPhysicType()==PhysicType::Trigger)
    {
        v_RigidBody->setCollisionFlags(v_RigidBody->getCollisionFlags()|btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
    m_IsEmpty=false;
    return true;
}

btBoxShape *BulletPhysicEngine::createCollShapeBox(const QVector<QVector4D>& a_Vertices, btTransform &a_Transformation)
{
    QVector3D v_Midpoint;
    QVector3D v_QtHalfExtends;
    BoundingBox::getBoxHalfExtends(a_Vertices,v_QtHalfExtends,v_Midpoint);

    btVector3 v_HalfExtents((btScalar)v_QtHalfExtends.x(),
                           (btScalar)v_QtHalfExtends.y(),
                           (btScalar)v_QtHalfExtends.z());

    a_Transformation.setOrigin(btVector3((btScalar)v_Midpoint.x(),
                                        (btScalar)v_Midpoint.y(),
                                        (btScalar)v_Midpoint.z()));
    btBoxShape *v_BoxShape=new btBoxShape(v_HalfExtents);
    return v_BoxShape;
}

DynamicCharacter *BulletPhysicEngine::createNewDynamicCharacter(IGeometryAccess* a_Geometry)
{
    DynamicCharacter* v_DynamicChar=new BulletDynamicCharacter(this,a_Geometry);
    return v_DynamicChar;
}

DynamicCharacterWithCam *BulletPhysicEngine::createNewDynamicCharacterWithCam(IGeometryAccess* a_Geometry)
{
    DynamicCharacterWithCam* v_DynChCam=new BulletDynamicCharacterWithCam(this,a_Geometry);
    return v_DynChCam;
}

PhysicObject *BulletPhysicEngine::createNewPhysicObject(IGeometryAccess *a_Geometry)
{
    BulletPhysicObject* v_PhysicObject=new BulletPhysicObject(a_Geometry);
    v_PhysicObject->m_PhysicEngine=this;
    a_Geometry->setPhysicObject(v_PhysicObject);
    return v_PhysicObject;
}

void BulletPhysicEngine::debugDraw()
{
    //TODO implement render interface
}

QMatrix4x4 BulletPhysicEngine::getTransformation(btTransform &a_BtMatrix)
{
    btScalar v_Matrix[16];
    a_BtMatrix.getOpenGLMatrix(v_Matrix);
    QMatrix4x4 v_4x4Matrix((float)v_Matrix[0],(float)v_Matrix[4],(float)v_Matrix[8],(float)v_Matrix[12]
            ,(float)v_Matrix[1],(float)v_Matrix[5],(float)v_Matrix[9],(float)v_Matrix[13]
            ,(float)v_Matrix[2],(float)v_Matrix[6],(float)v_Matrix[10],(float)v_Matrix[14]
            ,(float)v_Matrix[3],(float)v_Matrix[7],(float)v_Matrix[11],(float)v_Matrix[15]);
    return v_4x4Matrix;
}

#ifdef USE_PARALLEL_SOLVER
btThreadSupportInterface *BulletPhysicEngine::createSolverThreadSupport(int a_MaxNumThreads)
{
    //#define SEQUENTIAL
#ifdef SEQUENTIAL
    SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads",SolverThreadFunc,SolverlsMemoryFunc);
    SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
    threadSupport->startSPU();
#else

#ifdef _WIN32
    Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solverThreads",SolverThreadFunc,SolverlsMemoryFunc,a_MaxNumThreads);
    Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
    threadSupport->startSPU();
#elif defined (USE_PTHREADS)
    PosixThreadSupport::ThreadConstructionInfo solverConstructionInfo("solver", SolverThreadFunc,
                                                                      SolverlsMemoryFunc, a_MaxNumThreads);

    PosixThreadSupport* threadSupport = new PosixThreadSupport(solverConstructionInfo);

#else
    SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads",SolverThreadFunc,SolverlsMemoryFunc);
    SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
    threadSupport->startSPU();
#endif//_WIN32
#endif//SEQUENTIAL

    return threadSupport;
}
#endif //USE_PARALLEL_SOLVER

btTransform BulletPhysicEngine::getTransformation(QMatrix4x4 &a_QtMatrix)
{
    btTransform v_Transformation;
#ifdef BT_USE_DOUBLE_PRECISION
        btScalar v_MatrixScalar[16];
        for( int i=0;i<16;i++)
        {
            v_MatrixScalar[i]=(btScalar)a_QtMatrix.data()[i];
        }
        v_Transformation.setFromOpenGLMatrix(v_MatrixScalar);
#else
        v_Transformation.setFromOpenGLMatrix(a_QtMatrix.data());
#endif
        return v_Transformation;
}

btCollisionWorld *BulletPhysicEngine::getCollisionWorld()
{
    return (btCollisionWorld *)m_DynamicsWorld;
}

QString BulletPhysicEngine::getDebugMessage()
{
    return QString("Hello, I am the Bullet Physics Library that is running every frame :)");
}

btVector3 BulletPhysicEngine::QVector3DTobtVector3(const QVector3D a_Vector)
{
    return btVector3((btScalar)(a_Vector.x()),(btScalar)(a_Vector.y()),(btScalar)(a_Vector.z()));
}

QVector3D BulletPhysicEngine::btVector3ToQVector3D(const btVector3 &a_Vector)
{
    return QVector3D((float)(a_Vector.x()),(float)(a_Vector.y()),(float)(a_Vector.z()));
}

void BulletPhysicEngine::pauseSimulation()
{
    m_SimulationRunning=false;
}

PhysicObject *BulletPhysicEngine::rayTestClosestBody(QVector3D a_From, QVector3D a_To)
{
    btVector3 v_From=QVector3DTobtVector3(a_From);
    btVector3 v_To=QVector3DTobtVector3(a_To);

    btCollisionWorld::ClosestRayResultCallback v_ClosestBodyCallback(v_From,v_To);
    m_DynamicsWorld->rayTest(v_From,v_To,v_ClosestBodyCallback);

    PhysicObject* v_PhysicObject=0;

    if(v_ClosestBodyCallback.hasHit())
    {
        v_PhysicObject=(PhysicObject*)(v_ClosestBodyCallback.m_collisionObject->getUserPointer());
    }

    return v_PhysicObject;
}

QList<PhysicObject *> BulletPhysicEngine::rayTestAllBodys(QVector3D a_From, QVector3D a_To)
{
    btVector3 v_From=QVector3DTobtVector3(a_From);
    btVector3 v_To=QVector3DTobtVector3(a_To);

    btCollisionWorld::AllHitsRayResultCallback v_AllBodysCallback(v_From,v_To);
    m_DynamicsWorld->rayTest(v_From,v_To,v_AllBodysCallback);

    QList<PhysicObject*> r_BodyList;
    if(v_AllBodysCallback.hasHit())
    {
        PhysicObject* v_PhysicObject=0;
        int v_Counter=v_AllBodysCallback.m_collisionObjects.size();
        for(int i=0;i<v_Counter;i+=1)
        {
            v_PhysicObject=(PhysicObject*)(v_AllBodysCallback.m_collisionObjects.at(i)->getUserPointer());
            if(v_PhysicObject)
            {
                r_BodyList.append(v_PhysicObject);
                v_PhysicObject=0;
            }
        }
    }
    return r_BodyList;
}

void BulletPhysicEngine::deleteConstraints(PhysicObject *a_PhysicObjectA, PhysicObject *a_PhysicObjectB)
{
    QList<btTypedConstraint*> v_ConstraintList=((BulletPhysicObject*)a_PhysicObjectA)->removeConstraintPairs(a_PhysicObjectB);
    for(int i=0,j=v_ConstraintList.size();i<j;i++)
    {
        m_DynamicsWorld->removeConstraint(v_ConstraintList[i]);
        delete (v_ConstraintList[i]);
    }
}

void BulletPhysicEngine::removePhysicObject(PhysicObject *a_PhysicObject)
{
    BulletPhysicObject* v_BulletPhysicObject=dynamic_cast<BulletPhysicObject*>(a_PhysicObject);
    if(v_BulletPhysicObject)
    {
        if(v_BulletPhysicObject->m_RigidBody && v_BulletPhysicObject->m_PhysicEngine==this)
        {
            for(int i=0,j=v_BulletPhysicObject->m_ConstraintPairList.size();i<j;i++)
            {
                m_DynamicsWorld->removeConstraint(v_BulletPhysicObject->m_ConstraintPairList[i].m_Constraint);
            }
            this->m_DynamicsWorld->removeRigidBody(v_BulletPhysicObject->m_RigidBody);
            if(!(this->m_StaticPhysicObjects.removeAll(v_BulletPhysicObject)))
                if(!(this->m_DynamicPhysicObjects.removeAll(v_BulletPhysicObject)))
                    this->m_KinematicPhysicObjects.removeAll(v_BulletPhysicObject);
        }
    }
}

void BulletPhysicEngine::resumeSimulation()
{
    m_SimulationRunning=true;
}

void BulletPhysicEngine::setGravityForAllObjects(QVector3D a_Gravity)
{
    setGravityForNewObjects(a_Gravity);
    for(int i=0,j=m_DynamicPhysicObjects.size();i<j;i++)
    {
        m_DynamicPhysicObjects[i]->setGravity(a_Gravity);
    }
    for(int i=0,j=m_StaticPhysicObjects.size();i<j;i++)
    {
        m_StaticPhysicObjects[i]->setGravity(a_Gravity);
    }
    for(int i=0,j=m_KinematicPhysicObjects.size();i<j;i++)
    {
        m_KinematicPhysicObjects[i]->setGravity(a_Gravity);
    }
}

void BulletPhysicEngine::setFrequency(float a_Frequency)
{
    m_Frequency=a_Frequency;
}

void BulletPhysicEngine::setGravityForNewObjects(QVector3D a_Gravity)
{
    m_DynamicsWorld->setGravity(btVector3((btScalar)(a_Gravity.x()),(btScalar)(a_Gravity.y()),(btScalar)(a_Gravity.z())));
}

void BulletPhysicEngine::tick(int timeElapsedMS)
{
    setTicked(false);
    btScalar v_TimeElapsed;
    if(timeElapsedMS>41)
        v_TimeElapsed=(btScalar)0.041f;
    else
    v_TimeElapsed=((btScalar)timeElapsedMS/(btScalar)1000.);
    int v_MaxSubSteps=3;
    m_Frequency=v_TimeElapsed;
    if(m_Frequency>1.f/60.f){
        m_Frequency=1.f/60.f;
    }
    if(!m_IsEmpty && m_SimulationRunning)
    {
        m_DynamicsWorld->stepSimulation(v_TimeElapsed,v_MaxSubSteps,m_Frequency);
//        if(m_UpdateMatrix) //<<<jittering matrix update sometimes skips
//        {
            this->copyMatrixToGeometry();
//            m_UpdateMatrix=false;
//        }
    }
}

void BulletPhysicEngine::updateMatrix()
{
    m_UpdateMatrix=true;
}
