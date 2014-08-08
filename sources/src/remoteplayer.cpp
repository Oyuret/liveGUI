#include "include/remoteplayer.h"

RemotePlayer::RemotePlayer() : QObject()
{
}

void RemotePlayer::play(QString streamUrl, QString remoteUrl)
{
    QUrl requestUrl(remoteUrl + "/stream/play");
    QNetworkRequest request = createNetworkRequest(requestUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data;
    data.append("{\"url\":").append("\"").append(streamUrl).append("\"}");

    QNetworkReply *reply = remote.post(request,data);
    connectReply(reply);
}

void RemotePlayer::stop(QString remoteUrl)
{
    QUrl requestUrl(remoteUrl+"/stream/stop");
    QNetworkRequest request = createNetworkRequest(requestUrl);


    QNetworkReply *reply = remote.get(request);
    connectReply(reply);
}

QNetworkRequest RemotePlayer::createNetworkRequest(QUrl requestUrl) const
{
    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );
    return request;
}

void RemotePlayer::connectReply(QNetworkReply *reply)
{
    connect(reply, SIGNAL(finished()), this, SLOT(handleResponse()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void RemotePlayer::slotError(QNetworkReply::NetworkError)
{
}

void RemotePlayer::handleResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    reply->deleteLater();
}
