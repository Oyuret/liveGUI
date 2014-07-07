#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    QVector<QUrl> twitchUrl;
    twitchUrl.append(QUrl("https://api.twitch.tv/kraken/games/top?limit=100"));
    twitchUrl.append(QUrl("https://api.twitch.tv/kraken/streams"));

    //QVector<QUrl> azubuUrl;
    //azubuUrl.append(QUrl("https://api.twitch.tv/kraken/games/top?limit=100"));
    //azubuUrl.append(QUrl("https://api.twitch.tv/kraken/streams?game="));

    urls.append(twitchUrl);
}

void NetworkManager::fetch_games(API::SERVICE service)
{


    QNetworkRequest request;
    request.setUrl(urls[service][API::GAMES]);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_games()));
}

void NetworkManager::fetch_streams_by_game(QString game, API::SERVICE service)
{
    QNetworkRequest request;
    QUrlQuery query;

    switch(service) {
        case API::TWITCH:
        query.addQueryItem("game",game);
        query.addQueryItem("limit","100");
        break;
    }

    QUrl url = urls[service][API::STREAMS];
    url.setQuery(query);

    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_streams()));
}

/**
 * @brief NetworkManager::handle_twitch_games
 */
void NetworkManager::handle_twitch_games()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString data(reply->readAll());

    QScriptEngine engine;
    QScriptValue result = engine.evaluate("(" + data + ")");


    QScriptValue entries = result.property("top");
    QScriptValueIterator it(entries);

     while (it.hasNext()) {
        it.next();
        QScriptValue entry = it.value();

        QString name = entry.property("game").property("name").toString();
        QString viewers = entry.property("viewers").toString();
        API::SERVICE service = API::TWITCH;

        if(!name.isEmpty() && !viewers.isEmpty()) {
            emit add_game(name, viewers, service);
        }
    }

    // we are done with the reply. Let it be deleted
     reply->deleteLater();
}

void NetworkManager::handle_twitch_streams()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString data(reply->readAll());

    qDebug() << data;
}

