#include "physicengineticker.h"
#include "physicenginemanager.h"

PhysicEngineTicker::PhysicEngineTicker() : IdleObserver()
{
    mTimer.start();
}

void PhysicEngineTicker::doIt()
{
    long long time = mTimer.restart();
    PhysicEngineManager::tickPhysicEngines(time);
}
