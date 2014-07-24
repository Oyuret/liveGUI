#include "../include/networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    handlers.insert(TwitchHandler::getServiceName(),new TwitchHandler());
    setup_handlers();
}

NetworkManager::~NetworkManager()
{
    for(auto* handler : handlers.values()) {
        delete handler;
    }
}

void NetworkManager::setup_handlers()
{   
    for(auto* handler : handlers.values()) {
        QObject::connect(handler, SIGNAL(addGame(const Game&)),
                         this,SIGNAL(addGame(const Game&)));
        QObject::connect(handler, SIGNAL(addStream(const Stream&)),
                         this,SIGNAL(addStream(const Stream&)));
        QObject::connect(handler, SIGNAL(setPreview(const Stream&)),
                         this,SIGNAL(setPreview(const Stream&)));
        QObject::connect(handler, SIGNAL(resetPreview()),this,SIGNAL(resetPreview()));

        // fetching more games
        QObject::connect(handler, SIGNAL(fetch_next_games(QString,const Service&)),
                         this,SLOT(fetch_more_games(QString,const Service&)));

        //stream status
        QObject::connect(handler,SIGNAL(streamOnline(const Stream&)),
                         this,SIGNAL(streamOnline(const Stream&)));
        QObject::connect(handler,SIGNAL(streamOffline(const Stream&)),
                         this,SIGNAL(streamOffline(const Stream&)));
        QObject::connect(handler,SIGNAL(streamUncertain(const Stream&)),
                         this,SIGNAL(streamUncertain(const Stream&)));
    }

}

void NetworkManager::on_fetchGamesByService(const Service &service)
{
    QNetworkRequest request;
    QString serviceName = service.getServiceName();

    request.setUrl(handlers.value(serviceName)->getGamesUrl());
    request.setPriority(QNetworkRequest::HighPriority);

    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handleGames()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamsByGame(const Game &game)
{
    QNetworkRequest request;
    QString serviceName = game.getServiceName();

    request.setUrl(handlers.value(serviceName)->getStreamsUrl(game.getName()));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handleStreams()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamPreview(const Stream &stream)
{
    QNetworkRequest request;
    QString serviceName = stream.getServiceName();

    request.setUrl(handlers.value(serviceName)->getPreviewUrl(stream.getChannelName()));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handlePreview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamStatus(const Stream &stream)
{
    QNetworkRequest request;
    QString serviceName = stream.getServiceName();

    request.setUrl(handlers.value(serviceName)->getStatusUrl(stream.getChannelName()));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    AbstractHandler* handler = handlers.value(serviceName);
    connect(reply, &QNetworkReply::finished, [handler, stream, reply]() { handler->handleStatus(stream, reply); });
}

void NetworkManager::fetch_more_games(QString url, const Service &service)
{
    QNetworkRequest request;
    QString serviceName = service.getServiceName();
    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handleGames()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}


void NetworkManager::slotError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QMessageBox msgBox;
    QString text;

    text.append(reply->errorString());

    msgBox.setText(text);
    msgBox.exec();
}

