#include "scroller.h"
#include "scrolldialog.h"

Scroller::Scroller(QWidget *p_Parent)
    : QAbstractScrollArea(p_Parent)
{
    setViewport(new OpenGLWidget());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void Scroller::resizeEvent(QResizeEvent * event)
{
    OpenGLWidget * glw = dynamic_cast<OpenGLWidget *>(viewport());
    glw->resizeEvent(event);
}
#include <QScrollBar>
void Scroller::paintEvent(QPaintEvent *event)
{
    OpenGLWidget * glw = dynamic_cast<OpenGLWidget *>(viewport());
    glw->paintEvent(event);
//    QSize areaSize = viewport()->size();
//    QSize  widgetSize = size();

//   verticalScrollBar()->setPageStep(areaSize.height()/100);
//   horizontalScrollBar()->setPageStep(areaSize.width()/100);

//   verticalScrollBar()->setRange(0, widgetSize.height() - areaSize.height());
//   horizontalScrollBar()->setRange(0, widgetSize.width() - areaSize.width());

   horizontalScrollBar()->setPageStep(1);
   horizontalScrollBar()->setRange(0, 10);

   //updateWidgetPosition();
}
