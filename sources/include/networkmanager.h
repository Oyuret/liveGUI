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
    void addGame(const Game& game);
    void addStream(const Stream& stream);

    // signal preview
    void setPreview(const Stream& stream);
    void resetPreview();

    // signal stream status
    void streamOnline(const Stream& stream);
    void streamOffline(const Stream& stream);
    void streamUncertain(const Stream& stream);


public slots:
    void on_fetchGamesByService(const Service& service);
    void on_fetchStreamsByGame(const Game& game);
    void on_fetchStreamPreview(const Stream& stream);
    void on_fetchStreamStatus(const Stream& stream);

    // more games
    void fetch_more_games(QString url, const Service& service);

private slots:

    // Error handling
    void slotError(QNetworkReply::NetworkError);

};

#endif // NETWORKMANAGER_H
