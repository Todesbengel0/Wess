/*
 *  listener.h
 *  solar_oo
 *
 *  Created by bdr on 11.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <QPoint>

//! Klasse, von der abgeleitet werden muss, wenn man über Keyboard/Mouse Events informiert werden will
class Listener // Die Listener folgen weit gehend dem Observer Pattern
{
public:
    Listener();
    //! Behandlung von normalen Tasten
    virtual void keyboard(int, int) {}
    //! Behandlung von Tasten mit Modifier (ALT, Shift, ...)
    virtual void keyboardSpecial(int, int) {}
    //! Gibt Punkt in OpenGL 2D Koordinaten (0..width, 0..height) Tastendruck
    virtual void mouseLeftDown(const QPoint&) {}
    //! Gibt Punkt in OpenGL 2D Koordinaten (0..width, 0..height) Loslassen
    virtual void mouseLeftUp(const QPoint&) {}
    //! Gibt Punkt bei Bewegung mit gedrueckter linker Maustaste in OpenGL 2D Koordinaten (0..width, 0..height)
    //! Loslassen
    virtual void mouseMotion(const QPoint&) {}
};

#endif
