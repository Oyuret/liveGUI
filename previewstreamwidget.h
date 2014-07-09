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

public slots:
    void set_preview(QString streamer, QString game, QString viewers,
                     QString previewUrl, QString status, QString delay,
                     QString logoUrl, API::SERVICE service);
    void reset_preview();

private:
    Ui::previewStreamWidget *ui;

    // Let us have our own network mangager to download pics
    QNetworkAccessManager network;

private slots:
    void handle_logo();
    void handle_preview();
};

#endif // PREVIEWSTREAMWIDGET_H
