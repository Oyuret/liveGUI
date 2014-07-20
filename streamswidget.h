#ifndef STREAMSWIDGET_H
#define STREAMSWIDGET_H

#include <QWidget>
#include "global.h"
#include "streamitemwidget.h"
#include "stream.h"

namespace Ui {
class StreamsWidget;
}

class StreamsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StreamsWidget(QWidget *parent = 0);
    ~StreamsWidget();

signals:
    void back_to_games();
    void play(QString url);
    void fetch_preview(QString streamer, API::SERVICE service);
    void add_favorite(QString streamerName, QString name, QString url, API::SERVICE service);

    // preview
    void go_to_preview();

public slots:
    void add_stream(const Stream& stream);
    void clear_streams();

private slots:
    void on_backToGamesButton_clicked();

private:
    void connectStreamItemWidget(StreamItemWidget* streamItemWidget);
    Ui::StreamsWidget *ui;
};

#endif // STREAMSWIDGET_H
