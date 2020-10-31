#ifndef PHYSICENGINETICKER_H
#define PHYSICENGINETICKER_H

#include "idleobserver.h"
#include <QElapsedTimer>

//! Gibt den "Tick" an die Physikengine weiter
class PhysicEngineTicker : public IdleObserver
{
public:
    PhysicEngineTicker();
    virtual void doIt() override;

private:
    QElapsedTimer mTimer;
};

#endif // PHYSICENGINETICKER_H
