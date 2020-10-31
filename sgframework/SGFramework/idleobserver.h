/*
 *  idleobserver.h
 *  SGFramework
 *
 *  Created by bdr on 06.05.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _IDLEOBSERVER_H_
#define _IDLEOBSERVER_H_

//! Basisklasse für alle Objekte, die bei idle fuer Animationen gerufen werden wollen
//! Gerufen wird die Methode doIt, in der die ableitenden Klassen für jeden "Tick" ihre Aktionen vornehmen können
class IdleObserver
{
public:
    IdleObserver(bool delayed = false);
    void addMeNow();
    virtual void doIt() {}
};

#endif
