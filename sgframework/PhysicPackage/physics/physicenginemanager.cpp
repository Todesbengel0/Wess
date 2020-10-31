#include "physicenginemanager.h"
#include "physicengine.h"
#include "BulletPhysicEngine/bulletphysicengine.h"

QMutex PhysicEngineManager::m_MutexInstance;
PhysicEngineManager* PhysicEngineManager::m_Instance=0;

PhysicEngineManager::PhysicEngineManager()
{
    m_SlotsCounter=0;
}

PhysicEngineManager::~PhysicEngineManager()
{
    QMapIterator<int,PhysicEngine*> v_Iterator(m_PhysicEngineSlots);
    while(v_Iterator.hasNext())
    {
        v_Iterator.next();
        delete v_Iterator.value();
    }
    m_PhysicEngineSlots.clear();
}

int PhysicEngineManager::createNewPhysicEngineSlot(PhysicEngineName::PhysicEngineName a_PhysicEngineName)
{
    PhysicEngineManager* v_PhysicManager=getInstance();
    v_PhysicManager->m_MutexSlots.lock();
    PhysicEngine* v_PhysicEngine;
    switch(a_PhysicEngineName)
    {
    case PhysicEngineName::BulletPhysicsLibrary:
        v_PhysicEngine=new BulletPhysicEngine();
        break;
    default:
        v_PhysicManager->m_MutexSlots.unlock();
        return -1;      
    }
    v_PhysicManager->m_PhysicEngineSlots.insert(v_PhysicManager->m_SlotsCounter,v_PhysicEngine);
    int v_RetVal=v_PhysicManager->m_SlotsCounter;
    v_PhysicManager->m_SlotsCounter+=1;
    v_PhysicManager->m_MutexSlots.unlock();
    return v_RetVal;
}

void PhysicEngineManager::deleteInstance()
{
    if(PhysicEngineManager::m_Instance)
    {
        PhysicEngineManager::m_MutexInstance.lock();
        if(PhysicEngineManager::m_Instance)
        {
            delete PhysicEngineManager::m_Instance;
            PhysicEngineManager::m_Instance=0;
        }
        PhysicEngineManager::m_MutexInstance.unlock();
    }
}

void PhysicEngineManager::deletePhysicEngineSlot(int a_Slot)
{
    PhysicEngineManager* v_PhysicEngineManager=getInstance();
    if(v_PhysicEngineManager->m_PhysicEngineSlots.contains(a_Slot))
    {
        PhysicEngine* v_PhysicEngine=v_PhysicEngineManager->m_PhysicEngineSlots.value(a_Slot,0);
        if(v_PhysicEngine)
        {
            delete v_PhysicEngine;
        }
        v_PhysicEngineManager->m_PhysicEngineSlots.remove(a_Slot);
    }
}

void PhysicEngineManager::deletePhysicEngineSlot(PhysicEngine *a_PhysicEngine)
{
    PhysicEngineManager* v_PhysicEngineManager=getInstance();
    int v_Slot=v_PhysicEngineManager->m_PhysicEngineSlots.key(a_PhysicEngine,-1);
    if(v_Slot>=0)
    {
        deletePhysicEngineSlot(v_Slot);
    }
}

PhysicEngineManager *PhysicEngineManager::getInstance()
{
    if(!PhysicEngineManager::m_Instance)
    {
        PhysicEngineManager::m_MutexInstance.lock();
        if(!PhysicEngineManager::m_Instance)
        {
            PhysicEngineManager::m_Instance=new PhysicEngineManager();
        }
        PhysicEngineManager::m_MutexInstance.unlock();
    }
    return PhysicEngineManager::m_Instance;
}

int PhysicEngineManager::getNumberOfPhysicEngineSlots()
{
    PhysicEngineManager* v_PhysicManager=getInstance();
    v_PhysicManager->m_MutexSlots.lock();
    int v_RetVal=v_PhysicManager->m_SlotsCounter;
    v_PhysicManager->m_MutexSlots.unlock();
    return v_RetVal;
}

PhysicEngine *PhysicEngineManager::getPhysicEngineBySlot(int a_Slot)
{
    PhysicEngineManager* v_PhysicManager=getInstance();
    v_PhysicManager->m_MutexSlots.lock();
    if(a_Slot>=v_PhysicManager->m_SlotsCounter||a_Slot<0)
    {
        v_PhysicManager->m_MutexSlots.unlock();
        return 0;
    }
    PhysicEngine* v_RetVal=v_PhysicManager->m_PhysicEngineSlots.value(a_Slot,0);
    v_PhysicManager->m_MutexSlots.unlock();
    return v_RetVal;
}

void PhysicEngineManager::tickPhysicEngines(int a_TimeMS)
{
    PhysicEngineManager* v_PhysicManager=getInstance();
    for(int i=0;i<v_PhysicManager->m_SlotsCounter;i++){
        PhysicEngine*v_PhysicEngine=v_PhysicManager->m_PhysicEngineSlots.value(i,0);
        if(v_PhysicEngine){
            v_PhysicEngine->tick(a_TimeMS);
        }
    }
}
