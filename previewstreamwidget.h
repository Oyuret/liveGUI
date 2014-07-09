#ifndef PREVIEWSTREAMWIDGET_H
#define PREVIEWSTREAMWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "global.h"

namespace Ui {
class previewStreamWidget;
}

class previewStreamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit previewStreamWidget(QWidget *parent = 0);
    ~previewStreamWidget();

signals:
    void play(QString url);
    void add_favorite(QString streamerName, QString name, QString url, API::SERVICE service);

public slots:
    void set_preview(QString streamer, QString game, QString viewers,
                     QString previewUrl, QString status, QString delay,
                     QString logoUrl, QString url, API::SERVICE service);
    void reset_preview();

private:
    Ui::previewStreamWidget *ui;

    // Let us have our own network mangager to download pics
    QNetworkAccessManager network;
    QString url;
    API::SERVICE service;

private slots:
    void handle_logo();
    void handle_preview();
    void on_playButton_clicked();
    void on_favoriteButton_clicked();
};

#endif // PREVIEWSTREAMWIDGET_H
