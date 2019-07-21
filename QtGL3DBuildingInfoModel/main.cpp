#include <QApplication>
#include "ScrollDialog.h"
#include "OpenGLWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 1);
    QSurfaceFormat::setDefaultFormat(format);

    ScrollDialog dlg;
    dlg.show();

    return app.exec();
}
