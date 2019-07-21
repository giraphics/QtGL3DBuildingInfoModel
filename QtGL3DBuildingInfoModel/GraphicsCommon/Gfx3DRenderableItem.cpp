#include "Gfx3DRenderableItem.h"

GfxRenderableItem::GfxRenderableItem(Gfx3DScene* p_Scene, GfxRenderableItem* p_Parent, const QString& p_Name)
    : m_Scene(p_Scene),
      QObject(p_Parent),
      m_Center(),
      m_IsVisible(true),
      m_Program(0)
{
    assert(p_Scene);

    p_Scene->AddModel(this);

    setObjectName(p_Name);
}

void GfxRenderableItem::Initialize()
{
    foreach (QObject* objectItem, children())
    {
        GfxRenderableItem* model = dynamic_cast<GfxRenderableItem*>(objectItem);

        if (!model) continue;

        model->Initialize();
    }
}

void GfxRenderableItem::Render()
{
    if (!m_IsVisible) return;

    foreach (QObject* objectItem, children())
    {
        GfxRenderableItem* model = dynamic_cast<GfxRenderableItem*>(objectItem);

        if (!model || !model->m_IsVisible) continue;

        model->Render();
    }
}

void GfxRenderableItem::Rotate(float p_Angle, float p_X, float p_Y, float p_Z)
{
    m_Transformation.translate(m_Center);
    m_Transformation.rotate(p_Angle, p_X, p_Y, p_Z);
    m_Transformation.translate(-m_Center);
}

void GfxRenderableItem::Translate(float p_X, float p_Y, float p_Z)
{
    m_Transformation.translate(QVector3D(p_X, p_Y, p_Z));
}

void GfxRenderableItem::Scale(float p_X, float p_Y, float p_Z)
{
    m_Transformation.scale(QVector3D(p_X, p_Y, p_Z));
}

void GfxRenderableItem::Reset()
{
    m_Transformation.setToIdentity();
}

void GfxRenderableItem::ApplyTransformation()
{
    m_Scene->Transform().GetModelMatrix() *= m_Transformation;
}

QMatrix4x4 GfxRenderableItem::GetRelativeTransformations()
{
    return GetParentsTransformation(dynamic_cast<GfxRenderableItem*>(parent())) * m_Transformation;
}

QMatrix4x4 GfxRenderableItem::GetParentsTransformation(GfxRenderableItem* p_Parent)
{
    return p_Parent ? GetParentsTransformation(dynamic_cast<GfxRenderableItem*>(p_Parent->parent())) * p_Parent->m_Transformation : QMatrix4x4();
}

