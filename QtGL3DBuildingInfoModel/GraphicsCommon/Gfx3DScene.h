#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QMatrix4x4>
#include "GraphicsCommon/Gfx3DTransformation.h"

class GfxRenderableItem;
class QWindow;
class Gfx3DScene;

class Gfx3DScene : public QObject
{
    Q_OBJECT
public:
    Gfx3DScene(QObject *p_GLWindow, const QString& p_Name = ""); /* Surface must be OpenGL compatible */
    virtual ~Gfx3DScene();

    void Initialize();
    void Render();

    void AddModel(GfxRenderableItem* p_Model);
    void RemoveModel(GfxRenderableItem *p_Model);

    void Resize(int p_Width, int p_Height);
    void SetUpProjection();
    inline Gfx3DTransformation& Transform() { return m_Transform; }

private:
    int m_ScreenHeight;
    int m_ScreenWidth;

    QList<GfxRenderableItem*> m_ModelList;
    Gfx3DTransformation m_Transform;
    int m_Frame;
};
#endif // SCENE_H
