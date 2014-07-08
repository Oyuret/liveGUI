#ifndef PREVIEWSTREAMWIDGET_H
#define PREVIEWSTREAMWIDGET_H

#include <QWidget>

namespace Ui {
class previewStreamWidget;
}

class previewStreamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit previewStreamWidget(QWidget *parent = 0);
    ~previewStreamWidget();

private:
    Ui::previewStreamWidget *ui;
};

#endif // PREVIEWSTREAMWIDGET_H
