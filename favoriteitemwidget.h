#ifndef FAVORITEITEMWIDGET_H
#define FAVORITEITEMWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "global.h"

namespace Ui {
class FavoriteItemWidget;
}

class FavoriteItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteItemWidget(QString streamerName, QString name, QString url, QListWidgetItem* item, API::SERVICE service);
    ~FavoriteItemWidget();
    QString name;
    API::SERVICE service;

signals:
    void play(QString url);
    void preview(QString name, API::SERVICE service);
    void remove_favorite(QListWidgetItem* item);

private:
    Ui::FavoriteItemWidget *ui;
    QListWidgetItem* item;

public slots:
    void set_online();
    void set_offline();
    void set_checking();
private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_removeFavoriteButton_clicked();
};

#endif // FAVORITEITEMWIDGET_H
