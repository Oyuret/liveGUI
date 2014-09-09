#include "../include/azubuhandler.h"

AzubuHandler::AzubuHandler()
{
}

QUrl AzubuHandler::getGamesUrl() const
{
    return urls.at(AbstractHandler::GAMES);
}

QUrl AzubuHandler::getStreamsUrl(QString game) const
{
    return urls.at(AbstractHandler::STREAMS);
}

QUrl AzubuHandler::getPreviewUrl(QString channelName) const
{
    QString urlString = urls.at(PREVIEW).toString();
    urlString.append(channelName);

    QUrl url(urlString);
    return url;
}

QUrl AzubuHandler::getStatusUrl(QString channelName) const
{
    QString urlString = urls.at(STATUS).toString();
    urlString.append(channelName);

    QUrl url(urlString);
    return url;

}

void AzubuHandler::handleGames()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    AzubuGame game;
    game.read(responseObject);

    emit addGame(game);

}

void AzubuHandler::handleStreams() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data.mid(1,data.length()-2)));
    QJsonArray streamsArray = response.array();

    for(int i=0; i<streamsArray.size(); ++i) {

        QJsonObject streamJson = streamsArray[i].toObject();
        AzubuStream stream;

        // populate the stream
        stream.read(streamJson);

        // emit signal
        emit addStream(stream);
    }

}

void AzubuHandler::handlePreview() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data.mid(1,data.length()-3)));
    QJsonObject streamJson = response.object();

    QJsonValue online = streamJson["Online"];

    if(!online.isNull() && !online.isUndefined() && !online.toString().compare("0")==0) {
        AzubuStream stream;
        stream.read(streamJson);
        emit setPreview(stream);
    } else {
        emit resetPreview();
    }

}

void AzubuHandler::handleStatus(const Stream &stream, QNetworkReply* reply)
{

    if(reply->error() != QNetworkReply::NoError) {
        emit streamUncertain(stream);
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data.mid(1,data.length()-2)));
    QJsonObject responseObject = response.object();

    QJsonValue online = responseObject["Online"];

    if(online.isNull()) {
        emit streamUncertain(stream);
    } else if(online.isUndefined()) {
        emit streamUncertain(stream);
    } else if(online.toString().compare("0")==0){
        emit streamOffline(stream);
    } else {
        emit streamOnline(stream);
    }

    reply->deleteLater();
}


void AzubuHandler::AzubuGame::read(QJsonObject gameJson)
{
    QString viewers = "1337";
    QString channels = "1337";

    QString name = "League of Legends";

    QString serviceName = AzubuHandler::getServiceName();
    QString serviceLogoResource = ":/icons/icons/azubuicon.png";

    this->name = name;
    this->viewers = viewers;
    this->channels = channels;

    this->serviceName = serviceName;
    this->serviceLogoResource = serviceLogoResource;
}


void AzubuHandler::AzubuStream::read(QJsonObject streamJson)
{
    QString game = "League of Legends";
    QString viewers = streamJson["Viewer"].toString();

    QString displayName = streamJson["Name"].toString();
    QString channelName = streamJson["Name"].toString();

    QString status = "Not available";
    QString url = AzubuHandler::getBaseUrl().append(channelName);


    QString delay = "0";
    QString logoUrl = "Not available";

    QString previewUrl = "Not available";

    QString serviceName = AzubuHandler::getServiceName();
    QString serviceLogoResource = ":/icons/icons/azubuicon.png";

    this->displayName = displayName;
    this->channelName = channelName;
    this->status = status;
    this->url = url;
    this->game = game;
    this->viewers = viewers;
    this->delay = delay;
    this->logoUrl = logoUrl;
    this->previewUrl = previewUrl;

    this->serviceName = serviceName;
    this->serviceLogoResource = serviceLogoResource;

}
