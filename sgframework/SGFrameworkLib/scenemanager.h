#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "renderingcontext.h"
#include "rendermanager.h"
#include "mainwindow.h"
#include "scene.h"
#include "renderingstate.h"

namespace Ui
{
    class GLwithWidgets;
}

class MainWindow;

//! Ein zentrale Anlaufstelle im SGFramework (Singleton). Hier werden alle Szenen, Kontexte verwaltet. Zudem bietet die
//! Klasse Zugriff auf das übergeordnete Fenster, um bestimmte Attribute dort zu hinterlegen oder auch GUI-Elemente
//! hinzuzufügen. Zugriff muss immer über SceneManager::instance erfolgen!
//! Die Methode SceneManager::initScenes muss extern durch die jeweilige Anwendung implementiert werden!
class SceneManager
{
public:
    //! Rückgabe der (einzigen) Instanz
    static SceneManager* instance();

    void free();
    RenderingContext* getActiveContext();
    Scene* getActiveScene() {return mScenes[mActiveScene];}
    unsigned short addContext() {mContexts.append(new RenderingContext());return mContexts.size() - 1;}
    unsigned short addContext(RenderingContext* context) {mContexts.append(context);return mContexts.size() - 1;}
    unsigned short addScene(Node* pRoot) {mScenes.append(new Scene(pRoot));return mScenes.size() - 1;}
    unsigned short setActiveContext(unsigned short nr);
    unsigned short setActiveScene(unsigned short nr);
    void connectUiSignals(Ui::GLwithWidgets* pGLwithWidgets);
    void init();
    void setWindowReference(MainWindow* pQMainWindow, Ui::GLwithWidgets* pGLwithWidgets);
    RenderManager* getRenderManager();

    //! Anstoßen des Renderings über den RenderManager::renderScene
    void renderScene();

    //! Diese Methode wird im SGFramework nicht implementiert - sie muss von der Anwendung implementiert werden, die
    //! darin alle notwendigen Ressourcen anlegt und im SceneManager registriert (addContext, addScene).
    //! Diese Ressourcen sind Szenen, Kontexte inkl. der zugehörigen Kamera sowie Renderer. Mindestens jeweils ein
    //! Objekt (Szene, Kontext, Kamera) muss erzeugt und registriert werden. Weitere Szenen können angelegt werden und
    //! durch Listener-Objekte umgeschaltet werden. Optional können hier auch weitere GUI-Widgets angelegt werden und
    //! über SceneManager::getMainWindow "eingehangen" werden.
    void initScenes();

    //! Full Screen "Gaming mode"
    virtual void setFullScreen(bool fullScreen = true);

    //! Gibt an ob sich die aktuell im Full Screen "Gaming mode" befindet.
    bool isInFullScreenMode() const {return mMainWindow->isFullScreen();}

    //! Gibt das Hauptfenster der Anwendung zurück. Kann in SceneManager::initScenes verwendet werden, um zusätzliche
    //! GUI-Elemente einzuhängen oder die Attribute des Fenster zu verändern o.ä.
    QMainWindow* getMainWindow() const;


    //! Eine ShapeList pro root knoten einer Szene wenn der root knoten zweier
    //! Szenen gleich ist kann die ShapeList wiederverwendet werden und muss nicht
    //! neu erstellt und sortiert werden
    ShapeList& getShapeListForRoot(Node* root) {return mShapeLists[root];}

private:
    SceneManager();
    static SceneManager* sInstance;

    QVector<RenderingContext*> mContexts;
    unsigned short mCurContext;
    QVector<Scene*> mScenes;
    unsigned short mActiveScene;
    RenderManager* mRenderManager;
    MainWindow* mMainWindow;
    Ui::GLwithWidgets* mGLwithWidgets;
    QMap<Node*, ShapeList> mShapeLists;
};

//inline Scene* SceneManager::getActiveScene()
//{
//    return mScenes[mActiveScene];
//}

//inline unsigned short SceneManager::addContext()
//{
//    mContexts.append(new RenderingContext());
//    return mContexts.size() - 1;
//}

//inline unsigned short SceneManager::addContext(RenderingContext* context)
//{
//    mContexts.append(context);
//    return mContexts.size() - 1;
//}

//inline unsigned short SceneManager::addScene(Node* pRoot)
//{
//    mScenes.append(new Scene(pRoot));
//    return mScenes.size() - 1;
//}

//inline bool SceneManager::isInFullScreenMode() const
//{
//    return mMainWindow->isFullScreen();
//}

//inline ShapeList& SceneManager::getShapeListForRoot(Node* root)
//{
//    return mShapeLists[root];
//}

#endif // SCENEMANAGER_H
