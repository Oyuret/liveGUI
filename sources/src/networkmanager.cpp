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
        QObject::connect(handler, SIGNAL(add_game(const Game&)),
                         this,SIGNAL(add_game(const Game&)));
        QObject::connect(handler, SIGNAL(add_stream(const Stream&)),
                         this,SIGNAL(add_stream(const Stream&)));
        QObject::connect(handler, SIGNAL(set_preview(const Stream&)),
                         this,SIGNAL(set_preview(const Stream&)));
        QObject::connect(handler, SIGNAL(reset_preview()),this,SIGNAL(reset_preview()));

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

void NetworkManager::fetch_games(const Service &service)
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
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handle_games()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_streams_by_game(const Game &game)
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
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handle_streams()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_preview(const Stream &stream)
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
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handle_preview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_stream_status(const Stream &stream)
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
    connect(reply, &QNetworkReply::finished, [handler, stream, reply]() { handler->handle_status(stream, reply); });
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
    connect(reply, SIGNAL(finished()), handlers.value(serviceName), SLOT(handle_games()));
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

