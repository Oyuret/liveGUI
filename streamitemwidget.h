#ifndef STREAMITEMWIDGET_H
#define STREAMITEMWIDGET_H

#include <QWidget>
#include "stream.h"

namespace Ui {
class StreamItemWidget;
}

class StreamItemWidget : public QWidget
{
    Q_OBJECT

signals:
    void play(QString url);
    void fetch_preview(const Stream& stream);
    void add_favorite(const Stream& stream);
    void go_to_preview();

public:
    explicit StreamItemWidget(QWidget *parent = 0);
    StreamItemWidget(const Stream& stream);
    ~StreamItemWidget();

private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_favoriteButton_clicked();

private:
    Ui::StreamItemWidget *ui;
    Stream stream;
};

#endif // STREAMITEMWIDGET_H
