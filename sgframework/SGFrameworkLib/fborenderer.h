#ifndef FBORENDERER_H
#define FBORENDERER_H
#include "renderconfiguration.h"
#include "renderer.h"

class Drawable;

struct TexImageData
{
    GLint mInternalFormat;
    GLenum mFormat;
    GLenum mDataType;
    GLuint mRenderDest;   //!< Attachment, das benutzt werden soll, z.B. GL_COLOR_ATTACHMENT0
};

//! FBORenderer implementiert ein Offscreen-Rendering in ein oder mehrere FBO. Das FBO kann dann z.B. über eine FBOProperty an ein
//! Drawable "gehängt" werden. Es ist ein PreRenderer, der also vor dem eigentlichen Rendering durchlaufen wird.
class FBORenderer : public RenderConfiguration, public Renderer
{
public:
    //! Erzeugt einen neuen FBORenderer. Über \param sortDrawables kann zwischen einem SortedRenderingSGObjectVisitor
    //! und einem PreOrderRenderingSGObjectVisitor Besucher gewechselt werden. Die Verwendung von Sortiertem Rendering
    //! wird empfohlen da diese mehr Fps liefert. PreOrder Rendering wird hauptsächlich unterstützt um herauszufinden ob
    //! ein Problem durch das sortierte Rendern entsteht und Performanz Vergleiche zwischen den beiden Rendering
    //! Methoden zu erstellen.
    FBORenderer(bool sortDrawables = true);

    //! Destruktor löscht FBO + Textur + Renderbuffer
    virtual ~FBORenderer();

    //! \param renderTree ist der in das FBO zu rendernde teilbaum. \param fboOwnerDrawable ist das Drawable auf das die
    //! FBO texture anschließend gemapt/gerendert wird, dies wird benötigt das es beim rendern in das FBO deaktiviert
    //! werden muss, da es ansonsten das Bild verdecken würde.
    void init(Node *renderTree, int texDim, Drawable *fboOwnerDrawable, QVector<TexImageData> pTexImageData = {{GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0}});

    //! Rendert die Szene in das FBO
    virtual void renderScene() override;

    //! aktualisiert die Projektions Matrix der Camera. Überschreibt RenderConfiguration::update(), da die view matrix nicht
    //! berechnet wird. Dies geschieht bereits in renderScene()
    virtual void update(Camera* camera) override; // Methode aus RenderConfiguration überschreiben

    //! Getter für die Id der Textur in die das FBO rendert.
    virtual QVector<GLuint> getTexID() {return (mTexHandles);}

    //! Wird aufgerufen wenn das fenster Vergrösert/Verkleinert wird.
    virtual void reshape(int /*w*/, int /*h*/) override {}

    virtual void setViewport() override;

    //! Setzen der Auflösung - muss vor init gerufen werden!
    virtual void setRes(int pRes) {mFboHeight = mFboWidth = pRes;}

protected:
    virtual void initColorBuffer();
    virtual void initDepthBuffer();

private:
    GLuint mFBO;                        //!< Handle auf FBO
    GLuint mRenderbuffer;               //!< Handle zu Renderbuffer (Speicher), der an das FBO gebunden wird
    QVector<TexImageData> mTexImages;   //!< Daten der Textur, in die gerendert wird.
    QVector<GLuint> mTexHandles;        //!< Platz für die GL-Handles zu den TexImages
    GLuint mTexDim;                     //!< Dimension der Textur, in die gerendert wird.
    bool mValid;                        //!< gibt an ob das FBO korrekt erstellt und gebunden werden konnte

    GLsizei mFboWidth; //!< Die horizontale auflösung des FBOs
    GLsizei mFboHeight; //!< Die vertikale auflösung des FBOs

    //! Das Drawable auf das das FBO gerendert werden soll. Dies muss werend des renderns der FBO texturen deaktiviert
    //! werden da es sonst das bild überdecken würde!
    Drawable* mFboOwnerDrawable;

    //! Überprüft ob das Framebuffer-Object richtig erstellt und gebunden wurde.
    bool checkFramebufferStatus();
    void initFrameBufferAndRenderTarget();
};

//inline GLuint FBORenderer::getTexID()
//{
//    return (mRenderDest);
//}

//inline void FBORenderer::reshape(int /*w*/, int /*h*/)
//{
//}

#endif // FBORENDERER_H
