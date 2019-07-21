#ifndef SCROLLDIALOG_H
#define SCROLLDIALOG_H

#include <QDialog>
#include "OpenGLWidget.h"

namespace Ui {
class ScrollDialog;
}

class ScrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScrollDialog(QWidget *parent = 0);
    ~ScrollDialog();

    OpenGLWidget* GetGLWidget() const;

private:
    Ui::ScrollDialog *ui;
};

#endif // SCROLLDIALOG_H
