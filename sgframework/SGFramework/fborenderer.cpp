#include "fborenderer.h"
#include "scenemanager.h"
#include "sortedrenderingsgobjectvisitor.h"
#include "preorderrenderingsgobjectvisitor.h"

FBORenderer::FBORenderer(bool sortDrawables)
    : RenderConfiguration()
    , Renderer(sortDrawables ? static_cast<SGObjectVisitor*>(new SortedRenderingSGObjectVisitor())
                             : static_cast<SGObjectVisitor*>(new PreOrderRenderingSGObjectVisitor()))
{
    mValid = false;
    mRenderbuffer = mFBO = 0;
    mEnabled = true;
    mFboOwnerDrawable = nullptr;
    mFboWidth = 1024; // Breite und Höhe müssen (zumindest auf OSX) identisch sein
    mFboHeight = 1024;

#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
    glFunctions->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
}

//! Fügt die über renderTree erzeugte Szene im SceneManager hinzu, erzeugt das FBO mit allen Puffern.
void FBORenderer::init(Node* renderTree, int texDim, Drawable* fboOwnerDrawable, QVector<TexImageData> pTexImageData)
{
    Camera *lCam = new Camera();
    lCam->setPosition(.0,.0,.0);
    mContextNr = SceneManager::instance()->addContext();
    SceneManager::instance()->setActiveContext(mContextNr);
    mSceneNr = SceneManager::instance()->addScene(renderTree);
    SceneManager::instance()->setActiveScene(mSceneNr);
    SceneManager::instance()->getActiveContext()->setCamera(lCam);
    SceneManager::instance()->getActiveContext()->setRenderConfig(this);

    // Beim Rendermanager anmelden
    this->setAsPreRenderer();

    mTexImages = pTexImageData;
    mTexDim = texDim;
    mFboOwnerDrawable = fboOwnerDrawable;

    //Es wird immer dieselbe triviale Projektionsmatrix verwendet.
    QMatrix4x4 lProjMat;
    lProjMat.perspective(90.,1.,1.,1000.);
    mProjectionsMatrices.push_front(lProjMat);

    mViewportCount = 1;

    initFrameBufferAndRenderTarget();
    if (mTexImages[0].mRenderDest != GL_DEPTH_ATTACHMENT)
        initColorBuffer();
    else
        initDepthBuffer();

    mValid = checkFramebufferStatus();

    if (!mValid)
    {
        qDebug() << "For " << ((mTexDim == GL_TEXTURE_2D) ? "2D-Texture" : "Cube-Texture");
        qDebug() << "Framebuffer not properly set up.";
    }
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBORenderer::initColorBuffer()
{
    QVector<GLuint> lDrawbuffers;
    for (auto i = 0; i < mTexImages.length(); i++)
    {
        glFunctions->glBindTexture(mTexDim, mTexHandles[i]);
        if (mTexDim == GL_TEXTURE_CUBE_MAP)
        {
            glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            for (int j = 0; j < 6; j++)
            {
                glFunctions->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, mTexImages[i].mInternalFormat, mFboWidth, mFboHeight, 0, mTexImages[i].mFormat, mTexImages[i].mDataType, 0);
            }
        }
        else
            glFunctions->glTexImage2D(mTexDim, 0, mTexImages[i].mInternalFormat, mFboWidth, mFboHeight, 0, mTexImages[i].mFormat, mTexImages[i].mDataType, 0);

        glFunctions->glFramebufferTexture(GL_FRAMEBUFFER, mTexImages[i].mRenderDest, mTexHandles[i], 0);
        lDrawbuffers.append(mTexImages[i].mRenderDest);
    }
    glFunctions->glDrawBuffers(lDrawbuffers.length(), lDrawbuffers.constData());
    // Renderbuffer erzeugen und binden
    glFunctions->glGenRenderbuffers(1, &mRenderbuffer);
    glFunctions->glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
    if (mTexDim != GL_TEXTURE_CUBE_MAP)
        glFunctions->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderbuffer);
    glFunctions->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mFboWidth, mFboHeight);
}

void FBORenderer::initDepthBuffer()
{
    glFunctions->glTexImage2D(mTexDim, 0, mTexImages[0].mInternalFormat, mFboWidth, mFboHeight, 0, mTexImages[0].mFormat, mTexImages[0].mDataType, 0);
    glFunctions->glFramebufferTexture(GL_FRAMEBUFFER, mTexImages[0].mRenderDest, mTexHandles[0], 0);
    glFunctions->glDrawBuffer(GL_NONE);
}

void FBORenderer::initFrameBufferAndRenderTarget()
{
    // FBO erzeugen und binden
    glFunctions->glGenFramebuffers(1, &mFBO);
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    mTexHandles.resize(mTexImages.length());
    // Textur(en) als Rendertarget erzeugen
    for (auto i=0; i < mTexImages.length(); i++)
    {
        glFunctions->glGenTextures(1, &mTexHandles[i]);
        glFunctions->glBindTexture(mTexDim, mTexHandles[i]);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

//! Rendert die angelegte Szene in das FBO.
void FBORenderer::renderScene()
{
    if (!mValid)
    {
        qWarning() << "FBO Renderer called with non-valid FBO.";
        return;
    }

    // Das Drawable auf dass das FBO gerendert werden soll deaktivieren damit das bild nicht von ihm überdeckt wird!
    mFboOwnerDrawable->setEnabled(false);

    // FBO binden, um darin zu rendern
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glFunctions->glEnable(GL_DEPTH_TEST);
    glFunctions->glClearColor(0.0, 0.0, 0.0, 0.0);

    if (mTexDim == GL_TEXTURE_2D)
    {
        glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Sollte unnötig sein
        for (auto i=0; i<mTexImages.length(); i++)
        {
    #ifdef QT_OPENGL_ES_2
            glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER, mTexImages[i].mRenderDest, mTexHandles[i], 0, 0);
    #else
            glFunctions->glFramebufferTexture(GL_FRAMEBUFFER, mTexImages[i].mRenderDest, mTexHandles[i], 0);
    #endif
        }
        mViewMatrix = mFboOwnerDrawable->getWorldMatrix();
        mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f));
        Renderer::renderScene();
    }
    else // Cubemap
    {

        for (auto i=0; i<mTexImages.length(); i++)
        {
            for (int j = 0; j < 6; j++)
            {
                mViewMatrix.setToIdentity();

                glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER, mTexImages[i].mRenderDest,
                                                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, mTexHandles[i], 0);
                // Kamera für 6 verschiedene Richtungen setzen
                // Ein paar Worte zur Berechnung der lookAt-Matrix. Wir gehen in OpenGL normalerweise von rechtshändigem KS aus
                // Das Window-KS ist aber linkshändig. D.h. irgendwann muss man das umdrehen, hier geschehen durch die
                // Verwendung von 0,-1,0 als up-Vektor
                switch (GL_TEXTURE_CUBE_MAP_POSITIVE_X + j)
                {
                  case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0), QVector3D(0.0, -1.0, 0.0));
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(-1.0, 0.0, 0.0), QVector3D(0.0, -1.0, 0.0));
                    break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 0.0, 1.0));
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, -1.0, 0.0), QVector3D(0.0, 0.0, -1.0));
                    break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, -1.0, 0.0));
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                    mViewMatrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, -1.0), QVector3D(0.0, -1.0, 0.0));
                    break;
                }
                // Objekte rendern
                mViewMatrix = mViewMatrix * mFboOwnerDrawable->getWorldMatrix();

                glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Renderer::renderScene();
            }
        }
    }

    // Rendern in die Textur(en) beenden
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glFunctions->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFunctions->glBindTexture(mTexDim, 0);

    // Das Drawable, "auf das" das FBO gerendert werden soll wieder aktivieren.
    mFboOwnerDrawable->setEnabled(true);
}

void FBORenderer::update(Camera* camera)
{
    //Nichts zu tun, da View und Projection-Matrix schon gesetzt sind.
    Q_UNUSED(camera);
}

FBORenderer::~FBORenderer()
{
    if (mRenderbuffer)
    {
        glFunctions->glDeleteRenderbuffers(1, &mRenderbuffer);
    }
    if (mFBO)
    {
        glFunctions->glDeleteFramebuffers(1, &mFBO);
    }
    for (auto ltexHandles: mTexHandles)
    {
        glFunctions->glDeleteTextures(1, &(ltexHandles));
    }
}

bool FBORenderer::checkFramebufferStatus()
{
    GLenum status = (GLenum)glFunctions->glCheckFramebufferStatus(GL_FRAMEBUFFER);
#ifdef QT_OPENGL_ES
    return true;
#else
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        std::cout << "Framebuffer incomplete, incomplete attachment\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        std::cout << "Unsupported framebuffer format\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        std::cout << "Framebuffer incomplete, missing attachment\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        std::cout << "Framebuffer incomplete, attached images must have same dimensions\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        std::cout << "Framebuffer incomplete, attached images must have same format\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        std::cout << "Framebuffer incomplete, missing draw buffer\n" << std::endl;
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        std::cout << "Framebuffer incomplete, missing read buffer\n" << std::endl;
        return false;
    }
    return false;
#endif
}

void FBORenderer::setViewport()
{
    glFunctions->glViewport(0, 0, mFboWidth, mFboHeight);
}
