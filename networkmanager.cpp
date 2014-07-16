#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    // add handlers
    handlers.append(new TwitchHandler());

    // setup handlers
    setup_handlers();


}

NetworkManager::~NetworkManager()
{
    for(auto* handler : handlers) {
        delete handler;
    }
}

void NetworkManager::setup_handlers()
{
    // Forward the signals
    for(auto* handler : handlers) {
        QObject::connect(handler, SIGNAL(add_game(const Game&)),
                         this,SIGNAL(add_game(const Game&)));
        QObject::connect(handler, SIGNAL(add_stream(const Stream&)),
                         this,SIGNAL(add_stream(const Stream&)));
        QObject::connect(handler, SIGNAL(set_preview(const Stream&)),
                         this,SIGNAL(set_preview(const Stream&)));
        QObject::connect(handler, SIGNAL(reset_preview()),this,SIGNAL(reset_preview()));

        // fetching more games
        QObject::connect(handler, SIGNAL(fetch_next_games(QString,API::SERVICE)),this,SLOT(fetch_more_games(QString,API::SERVICE)));
    }


}

void NetworkManager::fetch_games(API::SERVICE service)
{
    QNetworkRequest request;
    request.setUrl(handlers.at(service)->getGamesUrl());
    request.setPriority(QNetworkRequest::HighPriority);

    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_games()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_streams_by_game(QString game, API::SERVICE service)
{
    QNetworkRequest request;

    request.setUrl(handlers.at(service)->getStreamsUrl(game));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_streams()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_preview(QString channelName, API::SERVICE service)
{
    QNetworkRequest request;

    request.setUrl(handlers.at(service)->getPreviewUrl(channelName));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_preview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_stream_status(QString channelName, API::SERVICE service, FavoriteItemWidget *item)
{
    item->set_checking();
    QNetworkRequest request;

    request.setUrl(handlers.at(service)->getStatusUrl(channelName));
    request.setPriority(QNetworkRequest::HighPriority);
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    AbstractHandler* handler = handlers.at(service);
    connect(reply, &QNetworkReply::finished, [handler, item, reply]() { handler->handle_status(item, reply); });
}

void NetworkManager::fetch_more_games(QString url, API::SERVICE service)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setPriority(QNetworkRequest::HighPriority);

    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_games()));
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

