#ifndef SCROLLER_H
#define SCROLLER_H
#include <QAbstractScrollArea>

class Scroller : public QAbstractScrollArea
{
public:
    Scroller(QWidget* p_Parent);
    virtual void paintEvent(QPaintEvent* p_Event);
    virtual void resizeEvent(QResizeEvent* p_Event);
};
#endif // SCROLLER_H
