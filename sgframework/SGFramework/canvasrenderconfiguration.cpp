#include "canvasrenderconfiguration.h"
#include "xmlparser.h"

CanvasRenderConfiguration::CanvasRenderConfiguration(bool pDefaultConfig, QString pPath) :
    RenderConfiguration(),
    mPos(0),
    mStereo(false),
    mDeltaEyePosition(QVector3D(0.03f, 0.f, 0.f))
{
    XmlParser parser;
    if (pDefaultConfig == false)
        parser.readFile(this, pPath, QString(""));
    else
        this->setParserData(parser.defaultCanvas());
    calculateviewportSizeFromCanvasCount();
}

void CanvasRenderConfiguration::setPath(const QString path, const QString caveID)
{
    XmlParser parser;
    qDebug() << "Reading Displayconfig in " << path << " config " << caveID;
    parser.readFile(this,path,caveID);
    calculateviewportSizeFromCanvasCount();
}

void CanvasRenderConfiguration::setStereo(const bool stereo)
{
    mStereo = stereo;
    calculateviewportSizeFromCanvasCount();
}

void CanvasRenderConfiguration::calculateviewportSizeFromCanvasCount()
{
    if(mStereo)
    {
        mViewportCount = mCanvasData.length() * 2;
        return;
    }
    mViewportCount = mCanvasData.length();
}

CanvasData CanvasRenderConfiguration::getOldCanvas() const
{
    return mOldCanvas;
}

void CanvasRenderConfiguration::setOldCanvas(const CanvasData &oldCanvas)
{
    mOldCanvas = oldCanvas;
}

void CanvasRenderConfiguration::setParserData(QVector<CanvasData> canvas)
{
    mCanvasData = canvas;
    calculateviewportSizeFromCanvasCount();
    addCanvas(mCanvasData);
    calculateCorners(mCanvasData);
}

void CanvasRenderConfiguration::addCanvas(QVector<CanvasData> canvasList)
{
    mCanvas.setCanvas(canvasList);
}

void CanvasRenderConfiguration::calculateCorners(QVector<CanvasData> canvasList)
{
    CanvasData startCanvas;
    /*schaut nach einer Leinwand bei der alle nötigten Eckkoordinaten
     *gegeben sind (muss mindestens einer sein!)*/
    for(int i = 0; i < canvasList.size(); i++)
    {
        if(canvasList.at(i).hasCoordinates)
        {
            startCanvas = canvasList.at(i);
            break;
        }
    }
    /*Berechnung des Winkel zwischen der Startleinwand am Boden
     * und der negativen z-Achse um den allgemeinen Rotationswinkel für
     * jede Leinwand zu bekommen*/
    QVector3D zAxis(0.f, 0.f, -1.f);
    QVector3D startVec = startCanvas.leftBottom - startCanvas.rightBottom;
    float startAngle = acos(QVector3D::dotProduct(zAxis, startVec)/(zAxis.length()*startVec.length()));
    //Umwandlung des Winkels von rad in deg
    startAngle = (startAngle*180.f)/M_PI;
    startAngle -= 90.f;

    float rotAngle = startAngle;

    //startCanvas ist die Leinwand welche alle nötigen Koordinaten beinhält
    //Leinwand für Leinwänd wird nach links durchgegangen und dessen Koordinaten berechnet
    CanvasData canvas = mCanvas.getLeftCanvas(startCanvas);
    mOldCanvas = startCanvas;
    while(canvas.id != mOldCanvas.id) //Wiederhole bis das linke Ende erreicht ist
    {
        qDebug() << canvas.id;
        if(!canvas.hasCoordinates) //Überspringe Berechnung wenn Koordinaten manuell gesetzt worden sind oder bereits berechnet
        {
            canvas.rightBottom = mCanvas.getRightCanvas(canvas).leftBottom;

            QVector3D leftBottomCorner(canvas.rightBottom.x(),canvas.rightBottom.y(),canvas.rightBottom.z()+canvas.width);

            leftBottomCorner -= canvas.rightBottom;

            QVector3D rotAxis = mCanvas.getRightCanvas(canvas).leftTop - mCanvas.getRightCanvas(canvas).leftBottom;
            rotAxis.normalize();

            //Rotation des Vectors um die y-Achse
            rotAngle += canvas.angle;

            QMatrix4x4 rotMatrix;
            rotMatrix.setToIdentity();
            rotMatrix.rotate(rotAngle, rotAxis);
            leftBottomCorner = rotMatrix*leftBottomCorner;

            leftBottomCorner += mCanvas.getRightCanvas(canvas).leftBottom;

            canvas.leftBottom = leftBottomCorner;

            QVector3D leftUpperCorner(canvas.leftBottom+canvas.height*rotAxis);
            leftUpperCorner.setY(canvas.leftBottom.y() + canvas.height);
            leftUpperCorner.setZ(canvas.leftBottom.z());

            canvas.leftTop = leftUpperCorner;
        }

        mCanvas.setCalculateCorners(canvas);
        mOldCanvas = canvas;
        canvas = mCanvas.getLeftCanvas(canvas);
    }
    //Nun der Durchgang Leinwand für Leinwand für die rechte Seite
    rotAngle = -startAngle;

    canvas = mCanvas.getRightCanvas(startCanvas);
    mOldCanvas = startCanvas;
    while(canvas.id != mOldCanvas.id) //Wiederhole solange das rechte Ende erreicht ist
    {
        if(!canvas.hasCoordinates)
        {
            //Die linke untere Koordinate der Leinwand ist die rechte untere Leinwand der linken Leinwand
            canvas.leftBottom = mCanvas.getLeftCanvas(canvas).rightBottom;
            QVector3D rightBottomCorner(canvas.leftBottom.x()+canvas.width,canvas.leftBottom.y(),canvas.leftBottom.z());
            rightBottomCorner -= canvas.leftBottom;

            //Rotation des Vectors um die y-Achse
            rotAngle += canvas.angle;
            QMatrix4x4 rotMatrix;
            rotMatrix.setToIdentity();
            rotMatrix.rotate(rotAngle, 0.f, 1.f, 0.f);
            rightBottomCorner = rightBottomCorner*rotMatrix;

            //Berechnung der rechten unteren Ecke
            rightBottomCorner += canvas.leftBottom;
            canvas.rightBottom = rightBottomCorner;
            //Zum Schluss die Berechnung der linken oberen Ecke

            QVector3D leftUpperCorner(canvas.leftBottom.x(),canvas.leftBottom.y() + canvas.height, canvas.leftBottom.z());
            canvas.leftTop = leftUpperCorner;
        }
        mCanvas.setCalculateCorners(canvas);
        mOldCanvas = canvas;
        canvas = mCanvas.getRightCanvas(canvas);
    }
}

void CanvasRenderConfiguration::setViewport()
{
    if(mStereo)
    {
        unsigned short index = mCurrentViewport%2;
        glFunctions->glViewport(mPos * mWidth/mCanvasData.length(), mHeight/2 * (index%2), mWidth/mCanvasData.length(), mHeight/2);
        mPos += index;
    }
    else
    {
        glFunctions->glViewport(mCurrentViewport * mWidth/mCanvasData.length(), 0, mWidth/mCanvasData.length(), mHeight);
    }
}

void CanvasRenderConfiguration::update(Camera* camera)
{
    if(camera->isProjectionMatrixCalcNeeded())
    {
        mProjectionsMatrices = mCanvas.getProjectionMatrices(camera, mDeltaEyePosition, mStereo);
    }
    if(camera->isViewMatrixCalcNeeded())
    {
        mViewMatrix.setToIdentity();
        mViewMatrix.lookAt(camera->getPosition(), camera->getPosition() + camera->getViewDir(), camera->getUpDir());
    }
}

void CanvasRenderConfiguration::cleanUp()
{
    RenderConfiguration::cleanUp();
    mPos = 0;
}

void CanvasRenderConfiguration::setDeltaEyePosition(const QVector3D& deltaEyePosition)
{
    mDeltaEyePosition = deltaEyePosition;
}
QVector3D CanvasRenderConfiguration::getDeltaEyePosition() const
{
    return mDeltaEyePosition;
}
