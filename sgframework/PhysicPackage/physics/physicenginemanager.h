#ifndef PHYSICENGINEMANAGER_H
#define PHYSICENGINEMANAGER_H
#include <QMap>
#include <QMutex>

class PhysicEngine;

//enum for different physic engines , bullet is the only atm
namespace PhysicEngineName {
enum PhysicEngineName{
    BulletPhysicsLibrary
};
}

//Is Responsible for the creation of instances of the physic engine
//it is a singleton and is responsible to tick all engines which were created
class PhysicEngineManager
{
private:
    QMutex m_MutexSlots;
    QMap<int,PhysicEngine*> m_PhysicEngineSlots;
    static PhysicEngineManager* m_Instance;
    static QMutex m_MutexInstance;
    int m_SlotsCounter;

private:
    PhysicEngineManager();
    ~PhysicEngineManager();
    PhysicEngineManager(const PhysicEngineManager &);
    PhysicEngineManager& operator=(const PhysicEngineManager &);

public:
    //Creates a new Physic Engine by name and returns the slot it was saved
    static int createNewPhysicEngineSlot(PhysicEngineName::PhysicEngineName a_PhysicEngineName);

    //deletes the instance of the manager, should be called if everything should be deleted
    static void deleteInstance();
    static void deletePhysicEngineSlot(int a_Slot);
    static void deletePhysicEngineSlot(PhysicEngine* a_PhysicEngine);

private:
    static PhysicEngineManager* getInstance();
public:
    static int getNumberOfPhysicEngineSlots();
    //gets a pointer to the physicengine at the given slot
    static PhysicEngine* getPhysicEngineBySlot(int a_Slot);

    //ticks the engine for a given time in ms
    static void tickPhysicEngines(int a_TimeMS);
};

#endif // PHYSICENGINEMANAGER_H
