#ifndef FAVORITEWIDGET_H
#define FAVORITEWIDGET_H

#include <QWidget>
#include <QSettings>
#include "favoriteitemwidget.h"
#include "stream.h"

namespace Ui {
class FavoriteWidget;
}

class FavoriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteWidget(QWidget *parent = 0);
    ~FavoriteWidget();

private:
    class FavoriteStream : public Stream {
    public:
        FavoriteStream();
        virtual ~FavoriteStream();

    };

signals:
    void play(QString url);
    void fetch_status(const Stream& stream, FavoriteItemWidget* item);
    void fetch_preview(const Stream& stream);
    void go_to_preview();

public slots:
    void add_favorite(const Stream& stream);
    void load_favorites();
    void save_favorites();

private slots:
    void remove_favorite(QListWidgetItem*item);
    void on_refreshStatusButton_clicked();

private:
    void connectFavoriteItemWidget(FavoriteItemWidget* favoriteItemWidget);
    bool alreadyInFavorites(QString url);
    Ui::FavoriteWidget *ui;

};

#endif // FAVORITEWIDGET_H
