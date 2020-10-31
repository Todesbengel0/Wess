#ifndef TESSPLANE_H
#define TESSPLANE_H

#include "simpleplane.h"
#include "listener.h"

class TessPlane : public SimplePlane, public Listener
{
public:
    TessPlane(float sizeX, float sizeY, unsigned int divs) : SimplePlane(sizeX, sizeY, divs) {init();}
    TessPlane(float sizeX, float sizeY)  : SimplePlane(sizeX, sizeY) {init();}
    TessPlane(float size)  : SimplePlane(size) {init();}
    virtual void render(Drawable* d) override;
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void keyboard(int key, int modifier) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

private:
    float mInnerLevels;
    float mOuterLevels;
    void init();
};

//inline TessPlane::TessPlane(float sizeX, float sizeY, unsigned int divs) : SimplePlane(sizeX, sizeY, divs)
//{
//    init();
//}

//inline TessPlane::TessPlane(float sizeX, float sizeY) : SimplePlane(sizeX, sizeY)
//{
//    init();
//}

//inline TessPlane::TessPlane(float size) : SimplePlane(size)
//{
//    init();
//}

#endif // TESSPLANE_H
