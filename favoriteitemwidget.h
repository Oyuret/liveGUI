#ifndef FAVORITEITEMWIDGET_H
#define FAVORITEITEMWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "stream.h"

namespace Ui {
class FavoriteItemWidget;
}

class FavoriteItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteItemWidget(const Stream& stream, QListWidgetItem* item);
    ~FavoriteItemWidget();

    Stream getStream() const;
    QString getUrl() const;

signals:
    void play(QString url);
    void fetch_preview(const Stream& stream);
    void remove_favorite(QListWidgetItem* item);
    void go_to_preview();

private:
    Ui::FavoriteItemWidget *ui;
    QListWidgetItem* item;
    Stream stream;

public slots:
    void set_online();
    void set_offline();
    void set_checking();
    void set_button_enabled();
    void set_button_disabled();
private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_removeFavoriteButton_clicked();
};

#endif // FAVORITEITEMWIDGET_H
