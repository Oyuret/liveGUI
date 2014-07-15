#ifndef PREVIEWSTREAMWIDGET_H
#define PREVIEWSTREAMWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <memory>
#include "global.h"
#include "stream.h"

namespace Ui {
class PreviewStreamWidget;
}

class PreviewStreamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewStreamWidget(QWidget *parent = 0);
    ~PreviewStreamWidget();

signals:
    void play(QString url);
    void add_favorite(QString displayName, QString name, QString url, API::SERVICE service);

public slots:
    void set_preview(std::shared_ptr<Stream> stream);
    void reset_preview();

private:
    Ui::PreviewStreamWidget *ui;
    std::shared_ptr<Stream> stream;

    // Let us have our own network mangager to download pics
    QNetworkAccessManager network;

private slots:
    void handle_logo();
    void handle_preview();
    void on_playButton_clicked();
    void on_favoriteButton_clicked();
};

#endif // PREVIEWSTREAMWIDGET_H
