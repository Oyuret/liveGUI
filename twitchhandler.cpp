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

        QString viewers = QString::number(qRound(top["viewers"].toDouble()));
        QString channels = QString::number(qRound(top["channels"].toDouble()));

        QJsonObject game = top["game"].toObject();
        QString name = game["name"].toString();

        emit add_game(name, viewers, channels, API::TWITCH);
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

        QJsonObject stream = streamsArray[i].toObject();

        QString game = stream["game"].toString();
        QString viewers = QString::number(qRound(stream["viewers"].toDouble()));

        QJsonObject channel = stream["channel"].toObject();

        QString status = channel["status"].toString().replace(QString("\n"),QString(""));
        QString url = channel["url"].toString();
        QString displayName = channel["display_name"].toString();
        QString channelName = channel["name"].toString();


        emit add_stream(displayName,channelName,status,game,viewers,url,API::TWITCH);
    }
    reply->deleteLater();

}

void TwitchHandler::handle_preview() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();


    QJsonObject stream = responseObject["stream"].toObject();
    QJsonValue online = responseObject["stream"];

    QString game = stream["game"].toString();
    QString viewers = QString::number(qRound(stream["viewers"].toDouble()));

    QJsonObject channel = stream["channel"].toObject();

    QString status = channel["status"].toString().replace(QString("\n"),QString(""));
    QString url = channel["url"].toString();
    QString displayName = channel["display_name"].toString();
    QString channelName = channel["name"].toString();
    QString delay = QString::number(qRound(channel["delay"].toDouble()));
    QString logoUrl = channel["logo"].toString();

    QJsonObject preview = stream["preview"].toObject();
    QString previewUrl = preview["medium"].toString();

    if(!online.isNull()) {
        emit set_preview(displayName,game,viewers,previewUrl,status,delay,logoUrl,url,API::TWITCH);
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
