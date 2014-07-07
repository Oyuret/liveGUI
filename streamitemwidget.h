#ifndef GAMEITEMWIDGET_H
#define GAMEITEMWIDGET_H

#include <QWidget>
#include "networkmanager.h"

namespace Ui {
class StreamItemWidget;
}

class StreamItemWidget : public QWidget
{
    Q_OBJECT

signals:
    void play(QString url);

public:
    explicit StreamItemWidget(QWidget *parent = 0);
    StreamItemWidget(QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service);
    ~StreamItemWidget();

private slots:
    void on_playStreamButton_clicked();

private:
    Ui::StreamItemWidget *ui;
};

#endif // GAMEITEMWIDGET_H
