#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H
#include "LinearMath/btIDebugDraw.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class BulletDebugDrawer: public btIDebugDraw, protected QOpenGLFunctions
{
public:
    BulletDebugDrawer();
    ~BulletDebugDrawer();
    void bindShader();
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
    void reportErrorWarning(const char *warningString);
    void draw3dText(const btVector3 &location, const char *textString);
    void setDebugMode(int debugMode);
    int getDebugMode() const;
    void setVPMatrix(QMatrix4x4 &a_VPMatrix);
private:
    QMatrix4x4 m_ViewProjectionMatrix;
    QOpenGLShaderProgram *m_ShaderProgram;
    int m_in_VertexLocation;
    int m_in_ColorLocation;
    int m_in_MatrixLocation;
};

#endif // DEBUGDRAWER_H
