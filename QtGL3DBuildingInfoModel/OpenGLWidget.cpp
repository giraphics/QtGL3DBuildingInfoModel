#include "OpenGLWidget.h"
#include<QElapsedTimer>
#include <QTemporaryDir>
#include <fstream>

QOpenGLFunctions* g_OpenGLFunctions = NULL;

#define FPS_TIME_START { \
                            static QElapsedTimer FPS; static int m_Frame;\
                            if (!FPS.isValid()) { FPS.start(); }

#define FPS_TIME_END        ++m_Frame; \
                            const int threshold = 3 * 1000; \
                            const int timing = FPS.elapsed(); \
                            if (timing > threshold) \
                            { \
                                qDebug("FPS: %f", m_Frame / 3.0f); \
                                m_Frame = 0; \
                                FPS.start(); \
                            } \
                        }
OpenGLWidget::OpenGLWidget(QWidget* p_Parent/* = NULL*/)
    : QOpenGLWidget(p_Parent),
      m_Scene(NULL)
{
}

void OpenGLWidget::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // Enable depth for BIM Model
    
    // Initiliazed the global pointer to suffice the reported link time errors for OpenGL functions in Fairlight
    g_OpenGLFunctions = context()->functions();
    
    SetUpBimModel();

    printVersionInformation();
}

void OpenGLWidget::resizeGL(int p_Width, int p_Height)
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, p_Width * retinaScale, p_Height * retinaScale);

    m_Scene->Resize(p_Width, p_Height);
}

void OpenGLWidget::paintGL()
{
    FPS_TIME_START

    QColor clearColor(15, 82, 186, 255);
    glClearColor(clearColor.redF(), clearColor.greenF(), 0.0f, clearColor.alphaF());

    m_DummyOrigin->Reset();
    static float rot = 0;
    m_DummyOrigin->Rotate(rot += 0.2, 0.0f, 1.0f, 0.0f);
    m_DummyOrigin->Translate(-1.3, -0.4f, 0.0f);
    m_DummyOrigin->Translate(1.3, -0.4f, 0.0f);
    m_DummyOrigin->Rotate(-90, 1.0f, 0.0f, 0.0f);
    m_DummyOrigin->Translate(-1.3, -0.4f, 0.0f);

    m_Scene->Render();
    FPS_TIME_END

    update();
}

void OpenGLWidget::SetUpBimModel()
{
    m_Scene = new Gfx3DScene(this, "BimModel");

    m_DummyOrigin = new Gfx3DRectangle(m_Scene, NULL, "MasterPositionPlane", !true, 3);
    m_DummyOrigin->SetWidth(.01);
    m_DummyOrigin->SetHeight(.01);
    m_DummyOrigin->SetColor(QColor(39, 171, 222));

    QTemporaryDir temporaryDir;
    QFile::copy(":/BinaryModel/BIMhouse.model", temporaryDir.path() + "/BIMhouse.model");
    //Read the file
    std::ifstream fileStream(QString(temporaryDir.path() + "/BIMhouse.model").toLatin1().data());
    fileStream.seekg (0, std::ios::beg);
    int vertexSize = 0;
    uint32_t m_NumMaterial = 0;
    while(!fileStream.eof())
    {
        // Material
        if (m_NumMaterial == 0)
        {
            fileStream.read((char*)(&m_NumMaterial), sizeof(uint32_t));
        }

        if (m_NumMaterial == 0) break;

        m_NumMaterial--;

        Gfx3DBimNode* currentChannel = new Gfx3DBimNode(m_Scene, 1, 13, 24, false, m_DummyOrigin, "Channels");
        currentChannel->Scale(0.15f, 0.15f, 0.15f);
        BimModelStruct& m_Model = currentChannel->GetModel();

        // material 1 - Color
        for(int i=0; i<4; i++)
        {
            fileStream.read((char*)&m_Model.m_Color[i], sizeof(GLfloat));
        }

        currentChannel->SetColor(QColor(m_Model.m_Color[0] * 255, m_Model.m_Color[1] * 255, m_Model.m_Color[2] * 255, m_Model.m_Color[3] * 255));

        // vertex count
        fileStream.read((char*)&m_Model.m_NumVertices, sizeof(uint32_t));

        // index count
        fileStream.read((char*)&m_Model.m_NumIndices, sizeof(uint32_t));

        vertexSize = 3 * m_Model.m_NumVertices;

        // vertex coordinates (x,y,z)
        m_Model.m_Vertices = new GLfloat[vertexSize]; // 3 for three vertice coord count

        for(int i=0; i<vertexSize; i++)
        {
            fileStream.read((char*)&m_Model.m_Vertices[i], sizeof(GLfloat));
        }

        // normal of each vertex (x,y,z)
        m_Model.m_Normals = new GLfloat[vertexSize]; // 3 for three normal coord count
        for(int i=0; i<vertexSize; i++)
        {
            fileStream.read((char*)&m_Model.m_Normals[i], sizeof(GLfloat));
        }

        // vertex indices
        m_Model.m_Indices = new uint32_t[m_Model.m_NumIndices]; // 3 for three vertice coord count
        for(int i=0; i<m_Model.m_NumIndices; i++)
        {
            fileStream.read((char*)&m_Model.m_Indices[i], sizeof(uint32_t));
        }
    }

    fileStream.close();

    m_Scene->Initialize();
}
