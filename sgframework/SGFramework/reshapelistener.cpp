/*
 *  ReshapeListener.cpp
 *  solar
 *
 *  Created by bdr on 27.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include "reshapelistener.h"
#include "openglwidget.h"

//! Diese Klasse wurde von der Klasse Listener getrennt, da die allermeisten Listener
//! nicht auf Reshape hören
ReshapeListener::ReshapeListener()
{
    OpenGLWidget::getInstance()->addReshapeListener(this);
}
