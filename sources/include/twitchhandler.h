#ifndef TWITCHHANDLER_H
#define TWITCHHANDLER_H

#include <QObject>
#include "abstracthandler.h"
#include "game.h"
#include "stream.h"

class TwitchHandler : public AbstractHandler
{
    Q_OBJECT
public:
    explicit TwitchHandler();
    virtual QUrl getGamesUrl() const;
    virtual QUrl getStreamsUrl(QString game) const;
    virtual QUrl getPreviewUrl(QString channelName) const;
    virtual QUrl getStatusUrl(QString channelName) const;

    static QString getServiceName() { return "twitch"; }

private:

    class TwitchGame : public Game {
    public:
        TwitchGame() : Game() {}
        virtual ~TwitchGame() {}
        virtual void read(QJsonObject gameJson);
    };

    class TwitchStream : public Stream {
    public:
        TwitchStream() : Stream() {}
        virtual ~TwitchStream() {}
        virtual void read(QJsonObject streamJson);
    };

    QVector<QUrl> urls {
                QUrl("https://api.twitch.tv/kraken/games/top?limit=100"),
                QUrl("https://api.twitch.tv/kraken/streams/"),
                QUrl("https://api.twitch.tv/kraken/streams/"),
                QUrl("https://api.twitch.tv/kraken/streams/")
    };

signals:

public slots:
    virtual void handleGames();
    virtual void handleStreams();
    virtual void handlePreview();
    virtual void handleStatus(const Stream& stream, QNetworkReply* reply);

};

#endif // TWITCHHANDLER_H
