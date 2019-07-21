#ifndef GFX_RENDERABLE_ITEM_H
#define GFX_RENDERABLE_ITEM_H

#include <QPointer>
#include <assert.h>
#include "GraphicsCommon/Gfx3DScene.h"

class GfxRenderableItem : public QObject
{
    Q_OBJECT
public:
    GfxRenderableItem(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent, const QString& p_Name = "");
    virtual ~GfxRenderableItem(){}

    virtual void Initialize() = 0;
    virtual void Render() = 0;

    void Rotate(float p_Angle, float p_X, float p_Y, float p_Z);
    void Translate(float p_X, float p_Y, float p_Z);
    void Scale(float p_X, float p_Y, float p_Z);
    void Reset();

    void SetCenter(const QVector3D& p_Center) { m_Center = p_Center; }
    QVector3D Center() const { return m_Center; }

    void SetVisible(bool p_IsVisible) { m_IsVisible = p_IsVisible; }
    bool IsVisible() const { return m_IsVisible; }

    void ApplyTransformation();
    QMatrix4x4 GetRelativeTransformations();
    QMatrix4x4 GetParentsTransformation(GfxRenderableItem* p_Parent);

protected:
    virtual void BuildGeometry() {}
    virtual void DestroyGeometry() {}

    unsigned int m_Program;
    Gfx3DScene* m_Scene;
    QMatrix4x4 m_Transformation;
    QVector3D m_Center;
    bool m_IsVisible;
    QPointer<GfxRenderableItem> m_Parent;
};
#endif // GFX_RENDERABLE_ITEM_H
