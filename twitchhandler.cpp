#include "twitchhandler.h"

TwitchHandler::TwitchHandler()
{
}

void TwitchHandler::handle_games()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonArray topArray = responseObject["top"].toArray();

    for(int i=0; i<topArray.size(); ++i) {

        QJsonObject top = topArray[i].toObject();
        Game* game = new TwitchGame();

        // populate the game
        game->read(top);

        // emit the signal
        emit add_game(game);
    }

    reply->deleteLater();
}

void TwitchHandler::handle_streams() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonArray streamsArray = responseObject["streams"].toArray();

    for(int i=0; i<streamsArray.size(); ++i) {

        QJsonObject streamJson = streamsArray[i].toObject();
        Stream* stream = new TwitchStream();

        // populate the stream
        stream->read(streamJson);

        // emit signal
        emit add_stream(stream);
    }
    reply->deleteLater();

}

void TwitchHandler::handle_preview() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();


    QJsonObject streamJson = responseObject["stream"].toObject();
    QJsonValue online = responseObject["stream"];

    if(!online.isNull()) {
        Stream* stream = new TwitchStream();
        stream->read(streamJson);
        emit set_preview(stream);
    } else {
        emit reset_preview();
    }

    reply->deleteLater();
}

void TwitchHandler::handle_status(FavoriteItemWidget* item, QNetworkReply* reply)
{
    item->set_button_enabled();

    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonValue online = responseObject["stream"];

    if(online.isNull()) {
        item->set_offline();
    } else {
        item->set_online();
    }

    reply->deleteLater();
}


void TwitchHandler::TwitchGame::read(QJsonObject gameJson)
{
    QString viewers = QString::number(qRound(gameJson["viewers"].toDouble()));
    QString channels = QString::number(qRound(gameJson["channels"].toDouble()));

    QJsonObject gameInner = gameJson["game"].toObject();
    QString name = gameInner["name"].toString();

    this->name = name;
    this->viewers = viewers;
    this->channels = channels;
    this->service = API::TWITCH;
}


void TwitchHandler::TwitchStream::read(QJsonObject streamJson)
{
    QString game = streamJson["game"].toString();
    QString viewers = QString::number(qRound(streamJson["viewers"].toDouble()));

    QJsonObject channel = streamJson["channel"].toObject();

    QString status = channel["status"].toString().replace(QString("\n"),QString(""));
    QString url = channel["url"].toString();
    QString displayName = channel["display_name"].toString();
    QString channelName = channel["name"].toString();

    QString delay = QString::number(qRound(channel["delay"].toDouble()));
    QString logoUrl = channel["logo"].toString();

    QJsonObject preview = streamJson["preview"].toObject();
    QString previewUrl = preview["medium"].toString();

    this->displayName = displayName;
    this->channelName = channelName;
    this->status = status;
    this->url = url;
    this->game = game;
    this->viewers = viewers;
    this->delay = delay;
    this->logoUrl = logoUrl;
    this->previewUrl = previewUrl;
    this->service = API::TWITCH;

}
