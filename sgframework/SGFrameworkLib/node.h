#ifndef NODE_H
#define NODE_H

/*! \mainpage Überblick über SGFramework
 * Diese Dokumentation gibt einen Überblick über diesen einfachen Szenengraphen.
 * Er wurde von Anfang mit dem Fokus auf Didaktik, insbesondere zur Vermittlung der Frage
 * wie Szenengraphen mit OpenGL arbeiten, gestaltet. Im Gegensatz zu frei verfügbaren, mächtigeren,
 * v.a. schnelleren Szenengraphen sind Design-Entscheidungen zu Gunsten des Verständnisses oder
 * auch zur besseren Erweiterbarkeit ohne Code-Änderungen (siehe Konzept von DrawableProperty) getroffen worden.
 * Gute Startpunkte zum Einlesen sind:
 * - Für den Aufbau des Graphen selbst: Die Klassen Node, Geometry, Drawable, Transformation
 * - Für die Schnittstelle oder für die Einbindung in das Windowing-System über Qt: Die Klassen OpenGLWidget und Window
 * - Für das eigentliche Rendering Window::paintGL, SceneManager::render und in der Folge
 * RenderManager::render sowie ScreenRenderer::render
 * - Für die Einbindung eigener Szenen SceneManager::initScenes
 * - Der Szenengraph hat eine Anbindung an die Physik-Engine Bullet, die separat dokumentiert wird.
 * Ein Startpunkt, um dort hineinzuschnuppern, ist Drawable::manipulatePhysic
 */

class SGObject;
class SGObjectVisitor;

//! Ein element des Szenengraphs/Baumes an das ein Szenengraphobjekte (SGObject) angehängt werden kann. Die trennung der
//! klasse SGObject und Node ermöglicht es SGObjects mehrmals inerhalb des Baumens einzuhängen.
class Node
{
public:
    Node(SGObject* sgObject);
    Node();
    virtual ~Node();

    //! Einfügen eines neuen Knotens, ggf. Verschieben des Vorhandenen in Sibling
    void addChild(Node* child);

    /*!
     * Durchlaufen des Graphen
     * \msc
     * Renderer, RootNode, RootSGObject, SGObjectVisitor, LeftChild, RightSibling;
     * Renderer=>RootNode [label="traverse()", URL="\ref Node::traverse()"];
     * RootNode=>RootSGObject [label="beforeTraverseChilds()", URL="\ref SGObject::beforeTraverseChilds()"];
     * RootSGObject=>SGObjectVisitor [label="beforeTraverseChilds()", URL="\ref
     * SGObjectVisitor::beforeTraverseChilds()"];
     * RootNode=>LeftChild [label="traverse()", URL="\ref Node::traverse()"];
     * ...;
     * RootNode=>RootSGObject [label="afterTraverseChilds()", URL="\ref SGObject::afterTraverseChilds()"];
     * RootNode=>RightSibling [label="traverse()", URL="\ref Node::traverse()"];
     * ...;
     * Renderer=>SGObjectVisitor [label="afterTraverseScene()", URL="\ref SGObjectVisitor::afterTraverseScene()"];
     * \endmsc
    */
    void traverse(SGObjectVisitor& visitor);

    //! Durchlaufen der Childs im Graph
    void traverseChilds(SGObjectVisitor& visitor);

    //! Durchlaufen der Siblings im Graph
    void traverseSiblings(SGObjectVisitor& visitor);

    //! Mutable Getter für das an diesen Node angehängte SGObject
    SGObject& getSGObject() {return *mSGObject;}

    //! Const Getter für das an diesen Node angehängte SGObject
    const SGObject& getSGObject() const {return *mSGObject;}

private:
    //! Das an diesen Node angehängte SGObject
    SGObject* mSGObject;

    //! Zeiger auf Kind
    Node* mLeftChild = nullptr;

    //! Zeiger auf Geschwister
    Node* mRightSibling = nullptr;
};

//inline SGObject& Node::getSGObject()
//{
//    return *mSGObject;
//}

//inline const SGObject& Node::getSGObject() const
//{
//    return *mSGObject;
//}

#endif
