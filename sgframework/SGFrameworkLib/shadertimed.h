#ifndef SHADERTIMED_H
#define SHADERTIMED_H

#include "shader.h"
#include "idleobserver.h"
#include <QElapsedTimer>

//! Ein Shader der eine Uniform-Variable 'Time' setzt um von der zeit abhängig animationen inerhalb des Shaders
//! ausführen zu können. Die Uniform-Variable 'Time' leuft von 0.0f bis 1.0f inerhalb von 'mMsecsPerIteration'
//! Millisekunden und startet dann wieder bei 0.0f.
class ShaderTimed : public Shader, public IdleObserver
{
public:
    ShaderTimed();
    virtual void setShaderUniforms() override;
    virtual void doIt() override;

    //! Setzt die Anzahl von Millisekunden innerhalb der die Uniform-Variable 'Time' von 0.0f bis 1.0f läuft,
    //! anschließend startet sie dann wieder bei 0.0f.
    inline void setMsecsPerIteration(int msecsPerIteration) {Q_ASSERT(msecsPerIteration>0); mMsecsPerIteration = msecsPerIteration;}

private:
    GLfloat mTime;
    QElapsedTimer mTimer;
    int mMsecsPerIteration = 1000;
};

//inline void ShaderTimed::setMsecsPerIteration(int msecsPerIteration)
//{
//    mMsecsPerIteration = msecsPerIteration;
//}

#endif //SHADERTIMED_H
