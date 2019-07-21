#include "Gfx3DShape.h"
#include <QtGui/QOpenGLFunctions>
#include<math.h>

#include "GraphicsCommon/Gfx3DProgramManager.h"
extern QOpenGLFunctions* g_OpenGLFunctions;

Gfx3DShape::Gfx3DShape(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent, const QString& p_Name)
    : GfxRenderableItem(p_Scene, p_Parent, p_Name),
      m_PositionBuffer(NULL),
      m_TextureBuffer(NULL)
{
}

static const char* vertexRectangle =
"#version 330 \n"
"layout(location = 0) in vec3 position; \n"
"uniform highp mat4 model; \n"
"uniform highp mat4 view; \n"
"uniform highp mat4 projection; \n"
" \n"
"void main() \n"
"{ \n"
"  gl_Position = projection * view * model * vec4(position, 1.0); \n"
"} \n";

static const char* fragmentFillRectangle =
"#version 330 \n"
"layout(location = 0, index = 0) out vec4 fragColor; \n"
" \n"
"void main() \n"
"{ \n"
"   fragColor = vec4(.30, 0.60, 0.60, 1.0); \n"
"} \n";

Gfx3DRectangle::Gfx3DRectangle(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent /*= NULL*/, const QString& p_Name /*= ""*/, bool p_IsFilled /*= false*/, GLfloat p_LineWidth /*= 1*/)
    : Gfx3DShape(p_Scene, p_Parent, p_Name),
      m_IsFilled(!p_IsFilled),
      m_LineWidth(p_LineWidth),
      m_Color(156, 56, 61)
{
}

void Gfx3DRectangle::Initialize()
{
    m_Program = Gfx3DProgramManager::Singleton()->ProgramLoad("Rectangle", vertexRectangle, fragmentFillRectangle);
    if (!m_Program)
    {
        DestroyGeometry();
    }
    m_Program->bind();
    BuildGeometry();

    m_PosAttr = m_Program->attributeLocation("position");

    m_MatrixModel = m_Program->uniformLocation("model");
    m_MatrixView = m_Program->uniformLocation("view");
    m_MatrixProjection = m_Program->uniformLocation("projection");
    m_ColAttr = m_Program->uniformLocation("colUniform");
    m_PositionBuffer->bind(); //#########

    // Create Vertex Array Object //#########
    m_Object.create();
    m_Object.bind();
    m_Program->enableAttributeArray(m_PosAttr);
    m_Program->setAttributeBuffer(m_PosAttr, GL_FLOAT, 0, 2, 0);
    //m_PositionBuffer->release(); // Uncommenting this make the drawing disappear. TODO PS

    // Release (unbind) all //#########
    m_Object.release();
    m_Program->release();

    Gfx3DShape::Initialize();
}

void Gfx3DRectangle::Render()
{
    m_Scene->Transform().PushMatrix();
    ApplyTransformation();

    m_Program = Gfx3DProgramManager::Singleton()->Program("Rectangle");
    m_Program->bind();

    m_Program->setUniformValue(m_MatrixProjection, m_Scene->Transform().GetProjectionMatrix());
    m_Program->setUniformValue(m_MatrixView, m_Scene->Transform().GetViewMatrix());
    m_Program->setUniformValue(m_MatrixModel, m_Scene->Transform().GetModelMatrix());

    // MacOSX: It seems the line with is only 1px max is supported
    GLint range[2];
    glGetIntegerv(GL_ALIASED_LINE_WIDTH_RANGE, range);
    glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, range);
    m_Object.bind();
    glDrawArrays(m_IsFilled? GL_TRIANGLE_STRIP : GL_LINE_STRIP, 0, (m_PositionBuffer->size() / sizeof(GLfloat)) / 2);
    m_Object.release();
    m_Program->release();

    Gfx3DShape::Render();
    m_Scene->Transform().PopMatrix();
}

void Gfx3DRectangle::SetFilled(bool p_IsFilled)
{
    m_IsFilled = p_IsFilled;
    BuildGeometry();
}

void Gfx3DRectangle::BuildGeometry()
{
    if (m_PositionBuffer == NULL)
    {
        m_PositionBuffer = new QOpenGLBuffer;
        m_PositionBuffer->create();
    }

    const float widthHalf = m_Width * 0.5f;
    const float heightHalf = m_Height * 0.5f;

    if (m_PositionBuffer->bind())
    {
        if (m_IsFilled)
        {
            GLfloat quad[][2] = {
                                    {-widthHalf, -heightHalf},
                                    { widthHalf, -heightHalf},
                                    {-widthHalf,  heightHalf},
                                    { widthHalf,  heightHalf},
                                };

            m_PositionBuffer->allocate(quad, sizeof(quad));
        }
        else
        {
            GLfloat ring[][2] = {
                                    {-widthHalf, -heightHalf},
                                    {-widthHalf,  heightHalf},
                                    { widthHalf,  heightHalf},
                                    { widthHalf, -heightHalf},
                                    {-widthHalf, -heightHalf},
                                };
            m_PositionBuffer->allocate(ring, sizeof(ring));
        }

        m_PositionBuffer->release();
    }
}

void Gfx3DRectangle::DestroyGeometry()
{
    m_PositionBuffer->destroy();
    delete m_PositionBuffer;
    m_PositionBuffer = NULL;

    m_TextureBuffer->destroy();
    delete m_TextureBuffer;
    m_TextureBuffer = NULL;
}

static const char* vertexBimNode =
"#version 330 \n"
"layout(location = 0) in vec4 position; \n"
"layout(location = 1) in vec3 faceDv; \n"
"uniform highp mat4 model; \n"
"uniform highp mat4 view; \n"
"uniform highp mat4 projection; \n"
"uniform mat3 normalMatrix; \n"
"uniform lowp vec4 colUniform; \n"
"out vec4 col; \n"
"void main() \n"
"{ \n"
"   vec3 LightPosition = vec3(vec4(0.0f, 0.0f, 30.0f, 1.0)); \n"
"   vec3 nNormal = normalize (normalMatrix * faceDv); \n"
"   vec3 eyeCoord = vec3(model * view * position); \n"
"   vec3 nLight = normalize(LightPosition - eyeCoord); \n"
"   float cosAngle = max( 0.0, dot( nNormal, nLight )); \n"
"   col = cosAngle * colUniform; \n"
"   gl_Position = projection * view * model * position; \n"
"} \n";

static const char* fragmentBimNode =
"#version 330 \n"
"layout(location = 0, index = 0) out vec4 fragColor; \n"
" \n"
"in highp vec4 col; \n"
"void main() \n"
"{ \n"
"   fragColor = col; \n"
"} \n";

Gfx3DBimNode::Gfx3DBimNode(Gfx3DScene* p_Scene, float p_Radius /*= 1*/, unsigned int p_Rings /*= 13*/, unsigned int p_Sectors /*= 24*/,
                           bool p_DrawAsCircle /*= false*/, GfxRenderableItem* p_Parent /*= NULL*/, const QString& p_Name /*= ""*/)
    : Gfx3DShape(p_Scene, p_Parent, p_Name),
      m_VertexIndex(NULL),
      m_Normals(NULL),
      m_Radius(p_Radius),
      m_Rings(p_DrawAsCircle ? 3 : p_Rings),
      m_Sectors(p_Sectors),
      m_DrawAsCircle(p_DrawAsCircle)
{
}

Gfx3DBimNode::~Gfx3DBimNode()
{
}

void Gfx3DBimNode::Initialize()
{
    m_Program = Gfx3DProgramManager::Singleton()->ProgramLoad("BimNode", vertexBimNode, fragmentBimNode);
    if (!m_Program)
    {
        DestroyGeometry();
    }
    m_Program->bind();
    BuildGeometry();

    m_PosAttr = m_Program->attributeLocation("position");
    m_NormalAttr = m_Program->attributeLocation("faceDv");

    m_MatrixModel = m_Program->uniformLocation("model");
    m_MatrixView = m_Program->uniformLocation("view");
    m_MatrixProjection = m_Program->uniformLocation("projection");
    m_ColAttr = m_Program->uniformLocation("colUniform");
    m_NormalMatrix = m_Program->uniformLocation("normalMatrix");

    // Create Vertex Array Object
    m_Object.create();
    m_Object.bind();

    m_PositionBuffer->bind();
    m_Program->enableAttributeArray(m_PosAttr);
    m_Program->setAttributeBuffer(m_PosAttr, GL_FLOAT, 0, 3, 0);

    m_Normals->bind();
    m_Program->enableAttributeArray(m_NormalAttr);
    m_Program->setAttributeBuffer(m_NormalAttr, GL_FLOAT, 0, 3, 0);

    m_VertexIndex->bind();

    // Release (unbind) all
    m_Object.release();
    //m_VertexIndex->release();
    //m_PositionBuffer->release(); // Uncommenting this make the drawing disappear. TODO PS
    m_Program->release();
    Gfx3DShape::Initialize();
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err;
    }
}

void Gfx3DBimNode::Render()
{
    m_Scene->Transform().PushMatrix();
    ApplyTransformation();

    m_Program = Gfx3DProgramManager::Singleton()->Program("BimNode");
    m_Program->bind();
    m_Program->setUniformValue(m_MatrixProjection, m_Scene->Transform().GetProjectionMatrix());
    m_Program->setUniformValue(m_MatrixView, m_Scene->Transform().GetViewMatrix());
    m_Program->setUniformValue(m_MatrixModel, m_Scene->Transform().GetModelMatrix());
    m_Program->setUniformValue(m_ColAttr, QVector4D(m_Color.redF(), m_Color.greenF(), m_Color.blueF(), m_Color.alphaF()));

    QMatrix4x4 matrixMV = m_Scene->Transform().GetModelViewMatrix();
    QMatrix3x3 normalMat = matrixMV.normalMatrix();
    m_Program->setUniformValue(m_NormalMatrix, normalMat);

    //m_PositionBuffer->bind();
    m_Object.bind();
    // glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_SHORT, &m_Indices[0]); // This will generate an error if used in VAO

    glDrawElements(GL_TRIANGLES, m_Model.m_NumIndices, GL_UNSIGNED_INT, 0);
    m_Object.release();
//    m_PositionBuffer->release();

    Gfx3DShape::Render();
    m_Scene->Transform().PopMatrix();
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err;
    }
}

void Gfx3DBimNode::BuildGeometry()
{
    if (m_PositionBuffer == NULL)
    {
        m_PositionBuffer = new QOpenGLBuffer;
        m_PositionBuffer->create();
    }

    if (m_PositionBuffer->bind())
    {
        // TODO REMOVE ME LATER
//        m_PositionBuffer->allocate(&m_Vertices.at(0), m_Vertices.size() * sizeof(float));
        m_PositionBuffer->allocate(&m_Model.m_Vertices[0], 3 * m_Model.m_NumVertices * sizeof(float));
        m_PositionBuffer->release();
    }

    if (m_VertexIndex == NULL)
    {
        m_VertexIndex = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_VertexIndex->create();
    }

    if (m_VertexIndex->bind())
    {
        m_VertexIndex->allocate(&m_Model.m_Indices[0], m_Model.m_NumIndices * sizeof(uint32_t));
        m_VertexIndex->release(QOpenGLBuffer::IndexBuffer);
    }

    if (m_Normals == NULL)
    {
        m_Normals = new QOpenGLBuffer;
        m_Normals->create();
    }

    if (m_Normals->bind())
    {
        m_Normals->allocate(&m_Model.m_Normals[0], 3 * m_Model.m_NumVertices * sizeof(float));
        m_Normals->release();
    }
}

void Gfx3DBimNode::DestroyGeometry()
{
}
