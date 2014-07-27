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

    class FavoriteStream : public Stream {
    public:
        FavoriteStream(QString displayName, QString channelName,
                       QString url, QString serviceName, QString logo);
        virtual ~FavoriteStream(){}
    };

public:
    explicit FavoriteWidget(QWidget *parent = 0);
    ~FavoriteWidget();

private:
    FavoriteItemWidget * getFavoriteItemWidget(int row) const;

signals:
    void play(QString url);
    void fetchStreamStatus(const Stream& stream);
    void fetchStreamPreview(const Stream& stream);
    void goToPreview();

public slots:
    void on_addFavorite(const Stream& stream);
    void on_loadFavorites();
    void on_saveFavorites();
    void on_streamOnline(const Stream& stream);
    void on_streamOffline(const Stream& stream);
    void on_streamUncertain(const Stream& stream);

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
