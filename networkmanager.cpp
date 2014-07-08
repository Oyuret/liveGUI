#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    QVector<QUrl> twitchUrl;
    twitchUrl.append(QUrl("https://api.twitch.tv/kraken/games/top?limit=100"));
    twitchUrl.append(QUrl("https://api.twitch.tv/kraken/streams/"));
    twitchUrl.append(QUrl("https://api.twitch.tv/kraken/streams/"));

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

void NetworkManager::fetch_preview(QString name, API::SERVICE service)
{
    QNetworkRequest request;
    QString urlString = urls[service][API::STREAMS].toString();

    switch(service) {
    case API::TWITCH:
        urlString.append(name);
        break;
    }

    QUrl url(urlString);

    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_preview()));
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

    QScriptEngine engine;
    QScriptValue result = engine.evaluate("(" + data + ")");

    QScriptValue entries = result.property("streams");
    QScriptValueIterator it(entries);

    while (it.hasNext()) {
        it.next();
        QScriptValue entry = it.value();

        QString streamer = entry.property("channel").property("display_name").toString();
        QString name = entry.property("channel").property("name").toString();
        QString status = entry.property("channel").property("status").toString().replace(QString("\n"),QString(""));
        QString game = entry.property("game").toString();
        QString viewers = entry.property("viewers").toString();
        QString url = entry.property("channel").property("url").toString();
        API::SERVICE service = API::TWITCH;

        if(!streamer.isEmpty()) {
            emit add_stream(streamer,name,status,game,viewers,url,service);
        }
    }

    reply->deleteLater();

}

void NetworkManager::handle_twitch_preview()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString data(reply->readAll());


    QScriptEngine engine;
    QScriptValue result = engine.evaluate("(" + data + ")");

    QScriptValue entry = result.property("stream");

    QString streamer = entry.property("channel").property("display_name").toString();
    QString game = entry.property("game").toString();
    QString viewers = entry.property("viewers").toString();
    QString previewUrl = entry.property("preview").property("medium").toString();
    QString status = entry.property("channel").property("status").toString().replace(QString("\n"),QString(""));
    QString delay = entry.property("channel").property("delay").toString();
    QString logoUrl = entry.property("channel").property("logo").toString();
    API::SERVICE service = API::TWITCH;


    if(!streamer.isEmpty()) {
        emit set_preview(streamer,game,viewers,previewUrl,status,delay,logoUrl,service);
    } else {
        emit reset_preview();
    }



    reply->deleteLater();

}

