#include "twitchhandler.h"

TwitchHandler::TwitchHandler()
{
}

void TwitchHandler::handle_games()
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
        QString channels_nr = entry.property("channels").toString();
        API::SERVICE service = API::TWITCH;

        if(!name.isEmpty() && !viewers.isEmpty()) {
            emit add_game(name, viewers, channels_nr, service);
        }
    }

    // we are done with the reply. Let it be deleted
    reply->deleteLater();
}

void TwitchHandler::handle_streams()
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

void TwitchHandler::handle_preview()
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
    QString url = entry.property("channel").property("url").toString();
    API::SERVICE service = API::TWITCH;


    if(!streamer.isEmpty()) {
        emit set_preview(streamer,game,viewers,previewUrl,status,delay,logoUrl,url,service);
    } else {
        emit reset_preview();
    }

    reply->deleteLater();
}
