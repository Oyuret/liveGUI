#ifndef PREVIEWSTREAMWIDGET_H
#define PREVIEWSTREAMWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
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
    void addFavorite(const Stream& stream);

public slots:
    void on_setPreview(const Stream& stream);
    void on_resetPreview();

private:
    void getPreviewImage(const Stream& stream);
    void getStreamLogo(const Stream& stream);
    bool previewIsValid() const;

    Ui::PreviewStreamWidget *ui;

    Stream stream;
    QNetworkAccessManager imageNetwork;

private slots:
    void handleStreamLogo();
    void handlePreviewImage();
    void on_playButton_clicked();
    void on_favoriteButton_clicked();
};

#endif // PREVIEWSTREAMWIDGET_H
