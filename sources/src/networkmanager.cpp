#include "../include/networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    handlers.insert(TwitchHandler::getServiceName(),new TwitchHandler());
    handlers.insert(AzubuHandler::getServiceName(),new AzubuHandler());
    setupHandlers();
}

NetworkManager::~NetworkManager()
{
    for(auto* handler : handlers.values()) {
        delete handler;
    }
}

void NetworkManager::setupHandlers()
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

AbstractHandler *NetworkManager::getHandler(QString serviceName) const
{
    return handlers.value(serviceName);
}

void NetworkManager::on_fetchGamesByService(const Service &service)
{
    AbstractHandler* handler = getHandler(service.getServiceName());
    QUrl requestUrl = handler->getGamesUrl();
    QNetworkRequest request = createNetworkRequest(requestUrl);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handler, SLOT(handleGames()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamsByGame(const Game &game)
{
    AbstractHandler* handler = getHandler(game.getServiceName());
    QUrl requestUrl = handler->getStreamsUrl(game.getName());
    QNetworkRequest request = createNetworkRequest(requestUrl);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handler, SLOT(handleStreams()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamPreview(const Stream &stream)
{
    AbstractHandler* handler = getHandler(stream.getServiceName());
    QUrl requestUrl = handler->getPreviewUrl(stream.getChannelName());
    QNetworkRequest request = createNetworkRequest(requestUrl);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handler, SLOT(handlePreview()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void NetworkManager::on_fetchStreamStatus(const Stream &stream)
{
    AbstractHandler* handler = getHandler(stream.getServiceName());
    QUrl requestUrl = handler->getStatusUrl(stream.getChannelName());
    QNetworkRequest request = createNetworkRequest(requestUrl);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, &QNetworkReply::finished, [handler, stream, reply]() { handler->handleStatus(stream, reply); });
}

void NetworkManager::fetch_more_games(QString url, const Service &service)
{

    AbstractHandler* handler = getHandler(service.getServiceName());
    QNetworkRequest request = createNetworkRequest(url);

    QNetworkReply *reply = get(request);
    connect(reply, SIGNAL(finished()), handler, SLOT(handleGames()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}

QNetworkRequest NetworkManager::createNetworkRequest(QUrl requestUrl) const
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

void NetworkManager::slotError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QMessageBox msgBox;
    QString text;

    text.append(reply->errorString());

    msgBox.setText(text);
    msgBox.exec();
}

