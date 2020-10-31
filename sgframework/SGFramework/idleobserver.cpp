/*
 *  idleobserver.cpp
 *  SGFramework
 *
 *  Created by bdr on 06.05.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include "idleobserver.h"
#include "openglwidget.h"

IdleObserver::IdleObserver(bool delayed)
{
    if (!delayed)
    {
        OpenGLWidget::getInstance()->addIdleObserver(this);
    }
}

void IdleObserver::addMeNow()
{
    OpenGLWidget::getInstance()->addIdleObserver(this);
}
