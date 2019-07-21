#ifndef GLMAINWIDGET_H
#define GLMAINWIDGET_H
#include<QOpenGLFunctions>
#include<QOpenGLWidget>
#include<QOpenGLFramebufferObject>
#include "GraphicsCommon/Gfx3DShape.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* p_Parent = NULL);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int p_Width, int p_Height);
    virtual void paintGL();
    void printVersionInformation();

private:
    void SetUpBimModel();

private:
    Gfx3DRectangle* m_DummyOrigin;
    Gfx3DTextureQuad* m_FboTextureQuad;
    Gfx3DScene* m_Scene;
    GLint m_DefaultBuffer;

    friend class Scroller;
};

#endif // GLMAINWIDGET_H
