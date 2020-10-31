#include "overlaywidget.h"

#include "openglwidget.h"

OverlayWidget::OverlayWidget(QWidget* parent) :
    QWidget(),
    mLayer(0)
{
    setParent(OpenGLWidget::getInstance());
    OpenGLWidget::getInstance()->addOverlayWidget(this);
}

OverlayWidget::~OverlayWidget()
{
}

void OverlayWidget::setLayer(unsigned int layer)
{
    mLayer = layer;
    OpenGLWidget::getInstance()->orderOverlayWidgets();
}

unsigned int OverlayWidget::getLayer() const
{
    return mLayer;
}
