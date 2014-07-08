#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

void NetworkManager::fetch_games(API::SERVICE service)
{


    QNetworkRequest request;
    request.setUrl(urls[service][API::GAMES]);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_games()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
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
    case API::AZUBU:
        break;
    }

    QUrl url = urls[service][API::STREAMS];
    url.setQuery(query);

    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_streams()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_preview(QString name, API::SERVICE service)
{
    QNetworkRequest request;
    QString urlString = urls[service][API::PREVIEW].toString();

    switch(service) {
    case API::TWITCH:
        urlString.append(name);
        break;
    case API::AZUBU:
        break;
    }

    QUrl url(urlString);

    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handle_twitch_preview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

/**
 * @brief NetworkManager::handle_twitch_games
 */
void NetworkManager::handle_twitch_games()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

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

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

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

void NetworkManager::slotError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QMessageBox msgBox;
    QString text;

    text.append(reply->errorString());

    msgBox.setText(text);
    msgBox.exec();
    reply->deleteLater();
}

