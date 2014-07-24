#ifndef STREAMSWIDGET_H
#define STREAMSWIDGET_H

#include <QWidget>
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
    void fetch_preview(const Stream& stream);
    void add_favorite(const Stream& stream);

    // preview
    void goToPreview();

public slots:
    void add_stream(const Stream& stream);

private slots:
    void on_backToGamesButton_clicked();

private:
    void connectStreamItemWidget(StreamItemWidget* streamItemWidget);
    void clear_streams();
    Ui::StreamsWidget *ui;
};

#endif // STREAMSWIDGET_H
