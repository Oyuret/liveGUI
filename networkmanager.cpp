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
        QObject::connect(handler, SIGNAL(add_game(QString,QString,QString,API::SERVICE)),
                         this,SIGNAL(add_game(QString,QString,QString,API::SERVICE)));
        QObject::connect(handler, SIGNAL(add_stream(QString,QString,QString,QString,QString,QString,API::SERVICE)),
                         this,SIGNAL(add_stream(QString,QString,QString,QString,QString,QString,API::SERVICE)));
        QObject::connect(handler, SIGNAL(set_preview(QString,QString,QString,QString,QString,QString,QString,QString,API::SERVICE)),
                         this,SIGNAL(set_preview(QString,QString,QString,QString,QString,QString,QString,QString,API::SERVICE)));
        QObject::connect(handler, SIGNAL(reset_preview()),this,SIGNAL(reset_preview()));
    }
}

void NetworkManager::fetch_games(API::SERVICE service)
{
    QNetworkRequest request;
    request.setUrl(urls[service][API::GAMES]);
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
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_streams()));
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
    request.setAttribute(
                QNetworkRequest::CacheLoadControlAttribute,
                QVariant( int(QNetworkRequest::AlwaysNetwork) )
                );

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handlers.at(service), SLOT(handle_preview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::fetch_stream_status(QString name, API::SERVICE service, FavoriteItemWidget *item)
{
    item->set_checking();
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


void NetworkManager::slotError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QMessageBox msgBox;
    QString text;

    text.append(reply->errorString());

    msgBox.setText(text);
    msgBox.exec();
    //reply->deleteLater();
}

