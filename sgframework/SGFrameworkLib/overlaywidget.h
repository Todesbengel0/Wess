#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>

class OverlayWidget : public QWidget
{
public:
    explicit OverlayWidget(QWidget* parent = 0);
    virtual ~OverlayWidget();

    void setLayer(unsigned int layer);
    unsigned int getLayer() const;

private:
    unsigned int mLayer;
};

#endif // OVERLAYWIDGET_H
