#include "include/remoteplayer.h"

RemotePlayer::RemotePlayer() : QObject()
{
}

void RemotePlayer::play(QString url)
{
    QUrl requestUrl("http://192.168.1.108/stream/play");
    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data;
    data.append("{\"url\":").append("\"").append(url).append("\"}");

    QNetworkReply *reply = remote.post(request,data);
    connect(reply, SIGNAL(finished()), this, SLOT(handleResponse()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void RemotePlayer::stop()
{
    QUrl requestUrl("http://192.168.1.108/stream/stop");
    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );


    QNetworkReply *reply = remote.get(request);
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
