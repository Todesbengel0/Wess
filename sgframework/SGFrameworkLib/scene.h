#ifndef SCENE_H
#define SCENE_H

#include "node.h"
#include "shapelist.h"

//! Szene enthält die Objekte einer Welt oder eines Levels. Damit auch verschiedene Szenen mit denselben Einstellungen
//! wie Kamera, etc. gerendert werden können, sind die Einstellungen für das Rendering nicht in der Szene selbst,
//! sondern im RenderingContext gespeichert. Eine Scene besteht daher eigentlich nur aus der Wurzel des Graphen.
class Scene
{
public:
    //! Erstellt eine Szene mit dem Root-Knoten \param pRoot
    Scene(Node* pRoot);

    //! Gibt den Root-Knoten des Szenengraphen/Baums zurück.
    inline Node* getRoot() {return mRoot;}

    //! Gibt die ShapeList der Szene zurück. Eine ShapeList kann zwischen mehreren Szenen geteilt werden wenn beide den
    //! gleichen Root-Knoten, und somit den gleichen Baum haben. Sodass ein Baum nur einmal sortiert werden muss.
    inline ShapeList& getShapeList() {return mShapeList;}

private:
    //! Der Root-Knoten des Szenengraphen/Baums.
    Node* mRoot;

    //! \see getShapeList()
    ShapeList& mShapeList;
};

//inline Node* Scene::getRoot()
//{
//    return mRoot;
//}

//inline ShapeList& Scene::getShapeList()
//{
//    return mShapeList;
//}

#endif // SCENE_H
