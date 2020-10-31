/*
 *  ReshapeListener.h
 *  solar
 *
 *  Created by bdr on 27.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _RESHAPELISTENER_H_
#define _RESHAPELISTENER_H_

//! Basisklasse für alle Observer, die über Reshapes informiert werden wollen
class ReshapeListener //Die Listener folgen weit gehend dem Observer Pattern
	{
public:
    ReshapeListener();
    virtual void reshape(int w, int h) {mWidth = w; mHeight = h;}
protected:
    unsigned int mWidth = 1;
    unsigned int mHeight = 1;
	};

#endif
