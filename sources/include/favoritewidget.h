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
        FavoriteStream(QString displayName, QString channelName,
                       QString url, QString serviceName, QString logo);
        virtual ~FavoriteStream(){}
    };

signals:
    void play(QString url);
    void fetch_status(const Stream& stream);
    void fetch_preview(const Stream& stream);
    void goToPreview();

public slots:
    void add_favorite(const Stream& stream);
    void load_favorites();
    void save_favorites();
    void streamOnline(const Stream& stream);
    void streamOffline(const Stream& stream);
    void streamUncertain(const Stream& stream);

private slots:
    void removeFavorite(QListWidgetItem*item);
    void on_refreshStatusButton_clicked();

private:
    void connectFavoriteItemWidget(FavoriteItemWidget* favoriteItemWidget);
    bool alreadyInFavorites(const Stream& stream) const;
    QList<FavoriteItemWidget*> findFavorite(const Stream& stream) const;
    Ui::FavoriteWidget *ui;

};

#endif // FAVORITEWIDGET_H
