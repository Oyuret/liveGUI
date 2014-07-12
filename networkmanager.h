#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <QUrl>
#include <QList>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrlQuery>
#include <QMessageBox>
#include "favoriteitemwidget.h"
#include "global.h"
#include "twitchhandler.h"

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT

private:
    QVector<QVector<QUrl>> urls {
        {
            {
                QUrl("https://api.twitch.tv/kraken/games/top?limit=100"),
                QUrl("https://api.twitch.tv/kraken/streams/"),
                QUrl("https://api.twitch.tv/kraken/streams/")
            }
        }
    };

public:
    explicit NetworkManager(QObject *parent = 0);
    virtual ~NetworkManager();

private:
    void setup_handlers();
    QVector<AbstractHandler*> handlers;

signals:

    // signal main window
    void add_game(Game* game);
    void add_stream(Stream* stream);

    // signal preview
    void set_preview(Stream* stream);
    void reset_preview();


public slots:
    void fetch_games(API::SERVICE service);
    void fetch_streams_by_game(QString game, API::SERVICE service);
    void fetch_preview(QString name, API::SERVICE service);
    void fetch_stream_status(QString name, API::SERVICE service, FavoriteItemWidget* item);

private slots:

    // Error handling
    void slotError(QNetworkReply::NetworkError);

};

#endif // NETWORKMANAGER_H
