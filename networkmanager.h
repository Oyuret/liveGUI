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

namespace API {

enum SERVICE {
    TWITCH,
    AZUBU
};

enum TYPE {
    GAMES,
    STREAMS
};

}

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT

private:
    QVector<QVector<QUrl>> urls;

public:
    explicit NetworkManager(QObject *parent = 0);

signals:

    // Signal twitch api manager
    void add_game(QString name, QString viewers, API::SERVICE service);

public slots:
    void fetch_games(API::SERVICE service);
    void fetch_streams_by_game(QString game, API::SERVICE service);

private slots:

    // Twitch
    void handle_twitch_games();
    void handle_twitch_streams();


};

#endif // NETWORKMANAGER_H