#ifndef FAVORITEWIDGET_H
#define FAVORITEWIDGET_H

#include <QWidget>
#include <QSettings>
#include "favoriteitemwidget.h"

namespace Ui {
class FavoriteWidget;
}

class FavoriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteWidget(QWidget *parent = 0);
    ~FavoriteWidget();

signals:
    void play(QString url);
    void fetch_status(QString name, API::SERVICE, FavoriteItemWidget* item);
    void fetch_preview(QString streamer, API::SERVICE service);

public slots:
    void add_favorite(QString displayName, QString channelName, QString url, API::SERVICE service);
    void load_favorites();
    void save_favorites();

private slots:
    void remove_favorite(QListWidgetItem*item);
    void on_refreshStatusButton_clicked();

private:
    Ui::FavoriteWidget *ui;

};

#endif // FAVORITEWIDGET_H
