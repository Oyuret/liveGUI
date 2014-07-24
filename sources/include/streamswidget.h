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
    void addStream(const Stream& stream);
    ~StreamsWidget();

signals:
    void goToGamesWidget();
    void play(QString url);
    void fetchStreamPreview(const Stream& stream);
    void addFavorite(const Stream& stream);

    // preview
    void goToPreview();

public slots:

private slots:
    void on_backToGamesButton_clicked();

private:
    void connectStreamItemWidget(StreamItemWidget* streamItemWidget);
    void clear_streams();
    Ui::StreamsWidget *ui;
};

#endif // STREAMSWIDGET_H
