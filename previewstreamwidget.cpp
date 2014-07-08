#include "previewstreamwidget.h"
#include "ui_previewstreamwidget.h"

previewStreamWidget::previewStreamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::previewStreamWidget)
{
    ui->setupUi(this);
}

previewStreamWidget::~previewStreamWidget()
{
    delete ui;
}
