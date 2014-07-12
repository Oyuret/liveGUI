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

signals:

public slots:
    virtual void handle_games();
    virtual void handle_streams();
    virtual void handle_preview();
    virtual void handle_status(FavoriteItemWidget *item, QNetworkReply* reply);

};

#endif // TWITCHHANDLER_H
