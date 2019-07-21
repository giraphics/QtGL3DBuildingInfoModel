#ifndef GFX3DSHAPE_H
#define GFX3DSHAPE_H

#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QColor>

#include "GraphicsCommon/Gfx3DRenderableItem.h"
#include <QOpenGLVertexArrayObject>

class Gfx3DShape : public GfxRenderableItem
{

public:
    Gfx3DShape(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent = 0, const QString& p_Name = "");
    ~Gfx3DShape(){}

protected:
    GLuint m_PosAttr;
    GLuint m_TexAttr;

    GLuint m_MatrixModel;
    GLuint m_MatrixView;
    GLuint m_MatrixProjection;

    QOpenGLBuffer* m_PositionBuffer;
    QOpenGLBuffer* m_TextureBuffer;
    QOpenGLShaderProgram* m_Program;
    QOpenGLVertexArrayObject m_object;
};

// Displays texture in 4 vertex quad
class Gfx3DTextureQuad : public Gfx3DShape
{
public:
    Gfx3DTextureQuad(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent = NULL, const QString& p_Name = "");

    void Initialize();
    void Render();
    virtual void BuildGeometry();
    virtual void DestroyGeometry();

    void SetTexture(GLuint p_Texture) { m_Texture = p_Texture; }
    GLuint Texture() const { return m_Texture; }

private:
    GLuint m_Texture;
};
class Vertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR Vertex();
  Q_DECL_CONSTEXPR explicit Vertex(const QVector2D &position);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const QVector2D& position() const;

  // OpenGL Helpers
  static const int PositionTupleSize = 2;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  QVector2D m_position;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex() {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector2D &position) : m_position(position) {}

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }
Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

// Rectangle and Square
class Gfx3DRectangle : public Gfx3DShape
{
public:
    Gfx3DRectangle(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent = NULL, const QString& p_Name = "", bool p_IsFilled = false, GLfloat p_LineWidth = 1);
    ~Gfx3DRectangle(){ DestroyGeometry(); }

    void Initialize();
    void Render();

    void SetWidth(float p_Width = 0.1f) { m_Width = p_Width; }
    float Width() const { return m_Width; }

    void SetHeight(float p_Height = 0.1f) { m_Height = p_Height; }
    float Height() const { return m_Height; }

    void SetColor(const QColor& p_Color) { m_Color = p_Color; }
    void SetFilled(bool p_IsFilled = true);
    bool isFilled() const { return m_IsFilled; }

private:
    void BuildGeometry();
    void DestroyGeometry();

    QOpenGLVertexArrayObject m_Object; // Todo: Consider moving to base class
    bool m_IsFilled;
    GLfloat m_LineWidth;
    GLuint m_ColAttr;
    QColor m_Color;
    float m_Width;
    float m_Height;
};

struct BimModelStruct
{
    GLfloat     m_Color[4];

    uint32_t    m_NumIndices; 	// index count
    uint32_t*   m_Indices;  // vertex indices

    uint32_t    m_NumVertices; // vertex count
    GLfloat*      m_Vertices;	// vertex coordinates (x,y,z)

    GLfloat*      m_Normals; 	// normal of each vertex (x,y,z)
};

class Gfx3DBimNode : public Gfx3DShape
{
public:
    Gfx3DBimNode(Gfx3DScene* p_Scene, float p_Radius = 1, unsigned int p_Rings = 13, unsigned int p_Sectors = 24,
                 bool p_DrawAsCircle = false, GfxRenderableItem* p_Parent = NULL, const QString& p_Name = "");
    virtual ~Gfx3DBimNode();

    void Initialize();
    void Render();

    void SetColor(const QColor& p_Color) { m_Color = p_Color; }
    BimModelStruct& GetModel() { return m_Model; }

private:
    void BuildGeometry();
    void DestroyGeometry();

    QOpenGLBuffer* m_VertexIndex;
    QOpenGLBuffer* m_Normals;

    GLuint m_ColAttr;
    GLuint m_NormalAttr;
    QColor m_Color;
    GLuint m_NormalMatrix;

    float m_Radius;
    unsigned short m_Rings;
    unsigned short m_Sectors;
    bool m_DrawAsCircle;

    QOpenGLVertexArrayObject m_Object; // Todo: Consider moving to base class
    std::vector<GLfloat> m_Vertices;
    //std::vector<GLfloat> m_Normals;
    std::vector<GLfloat> m_TexCoords;
    std::vector<GLushort> m_Indices;

    //////////////////////////////////////
    BimModelStruct m_Model;
    uint32_t m_NumMaterial;
};


#endif // GFX3D3DSHAPE_H
