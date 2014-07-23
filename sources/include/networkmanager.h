#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHash>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrlQuery>
#include <QMessageBox>
#include "favoriteitemwidget.h"
#include "twitchhandler.h"
#include "service.h"

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = 0);
    virtual ~NetworkManager();

private:
    void setup_handlers();
    QHash<QString,AbstractHandler*> handlers;

signals:

    // signal main window
    void add_game(const Game& game);
    void add_stream(const Stream& stream);

    // signal preview
    void set_preview(const Stream& stream);
    void reset_preview();

    // signal stream status
    void streamOnline(const Stream& stream);
    void streamOffline(const Stream& stream);
    void streamUncertain(const Stream& stream);


public slots:
    void fetch_games(const Service& service);
    void fetch_streams_by_game(const Game& game);
    void fetch_preview(const Stream& stream);
    void fetch_stream_status(const Stream& stream);

    // more games
    void fetch_more_games(QString url, const Service& service);

private slots:

    // Error handling
    void slotError(QNetworkReply::NetworkError);

};

#endif // NETWORKMANAGER_H
