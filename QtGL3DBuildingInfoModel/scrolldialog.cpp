#include "scrolldialog.h"
#include "ui_scrolldialog.h"
#include <QLabel>
ScrollDialog::ScrollDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScrollDialog)
{
    ui->setupUi(this);
}

ScrollDialog::~ScrollDialog()
{
    delete ui;
}

OpenGLWidget *ScrollDialog::GetGLWidget() const
{
    return NULL; //ui->glWidget;
}
