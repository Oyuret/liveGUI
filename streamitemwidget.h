#ifndef STREAMITEMWIDGET_H
#define STREAMITEMWIDGET_H

#include <QWidget>
#include <memory>
#include "global.h"
#include "stream.h"

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
    StreamItemWidget(std::shared_ptr<Stream> stream);
    ~StreamItemWidget();

private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_favoriteButton_clicked();

private:
    Ui::StreamItemWidget *ui;
    std::shared_ptr<Stream> stream;
};

#endif // STREAMITEMWIDGET_H
