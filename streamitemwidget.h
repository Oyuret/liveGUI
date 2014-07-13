#ifndef STREAMITEMWIDGET_H
#define STREAMITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class StreamItemWidget;
}

class StreamItemWidget : public QWidget
{
    Q_OBJECT

signals:
    void play(QString url);
    void fetch_preview(QString streamer, API::SERVICE);
    void add_favorite(QString streamerName, QString name, QString url, API::SERVICE service);
    void go_to_preview();

public:
    explicit StreamItemWidget(QWidget *parent = 0);
    StreamItemWidget(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service);
    ~StreamItemWidget();

private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_favoriteButton_clicked();

private:
    Ui::StreamItemWidget *ui;
    API::SERVICE service;
    QString name;
};

#endif // STREAMITEMWIDGET_H
