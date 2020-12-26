#ifndef SHADOWMAPRENDERINGSGOBJECTVISITOR_H
#define SHADOWMAPRENDERINGSGOBJECTVISITOR_H

#include "sortedrenderingsgobjectvisitor.h"

class ShadowMapRenderingSGObjectVisitor : public SortedRenderingSGObjectVisitor
{
public:
    inline void setShadedObjectShader(Shader* shadedObjectShader) {Q_ASSERT(shadedObjectShader);mShadedObjectShader = shadedObjectShader;}

    virtual void afterTraverseScene() override;

private:
    Shader* mShadedObjectShader;
};

#endif // SHADOWMAPRENDERINGSGOBJECTVISITOR_H
