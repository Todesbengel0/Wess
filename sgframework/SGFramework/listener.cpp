/*
 *  listener.cpp
 *  solar_oo
 *
 *  Created by bdr on 11.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include "openglwidget.h"
#include "listener.h"

Listener::Listener()
{
    OpenGLWidget::getInstance()->addListener(this);
}
