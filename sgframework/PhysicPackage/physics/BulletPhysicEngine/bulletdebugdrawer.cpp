#include "bulletdebugdrawer.h"
#include <QColor>
#include "opengl.h"

BulletDebugDrawer::BulletDebugDrawer()
{
    m_ShaderProgram = 0;
}

BulletDebugDrawer::~BulletDebugDrawer()
{
    delete m_ShaderProgram;
    m_ShaderProgram = 0;
}

void BulletDebugDrawer::bindShader()
{
    if (!m_ShaderProgram)
    {
        QString v_VertexShader = "#version 330\n"
                                 "uniform mat4 in_ViewProjectionMatrix;\n"
                                 "in vec4 in_Vertex;\n"
                                 "void main()\n"
                                 "{"
                                 "gl_Position=in_ViewProjectionMatrix*in_Vertex;\n"
                                 "}\n";
        QString v_FragmentShader = "#version 330\n"
                                   "uniform vec4 in_Color;\n"
                                   "out vec4 out_FragmentColor;\n"
                                   "void main ()\n"
                                   "{\n"
                                   "out_FragmentColor=in_Color;\n"
                                   "}\n";
        m_ShaderProgram = new QOpenGLShaderProgram();
        m_ShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, v_VertexShader);
        m_ShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, v_FragmentShader);
        m_ShaderProgram->link();

        m_in_VertexLocation = m_ShaderProgram->attributeLocation("in_Vertex");
        m_in_ColorLocation = m_ShaderProgram->uniformLocation("in_Color");
        m_in_MatrixLocation = m_ShaderProgram->uniformLocation("in_ViewProjectionMatrix");
    }
    m_ShaderProgram->bind();
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    GLfloat v_Vertices[] = {from.x(), from.y(), from.z(), to.x(), to.y(), to.z()};
    QColor v_Color(255.f * color.x(), 255.f * color.y(), 255.f * color.z(), 255.f);

    m_ShaderProgram->enableAttributeArray(m_in_VertexLocation);
    m_ShaderProgram->setAttributeArray(m_in_VertexLocation, v_Vertices, 3);
    m_ShaderProgram->setUniformValue(m_in_ColorLocation, v_Color);
    m_ShaderProgram->setUniformValue(m_in_MatrixLocation, m_ViewProjectionMatrix);

    glFunctions->glDrawArrays(GL_LINES, 0, 2);

    m_ShaderProgram->disableAttributeArray(m_in_VertexLocation);
}

void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                         int lifeTime, const btVector3& color)
{
    ( void )PointOnB;
    ( void )normalOnB;
    ( void )distance;
    ( void )lifeTime;
    ( void )color;
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
    ( void )warningString;
}

void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    ( void )location;
    ( void )textString;
}

void BulletDebugDrawer::setDebugMode(int debugMode)
{
    ( void )debugMode;
}

int BulletDebugDrawer::getDebugMode() const
{
    return 1;
}

void BulletDebugDrawer::setVPMatrix(QMatrix4x4& a_VPMatrix)
{
    m_ViewProjectionMatrix = a_VPMatrix;
}
