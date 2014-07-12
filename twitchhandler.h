#ifndef TWITCHHANDLER_H
#define TWITCHHANDLER_H

#include <QObject>
#include "abstracthandler.h"

class TwitchHandler : public AbstractHandler
{
    Q_OBJECT
public:
    explicit TwitchHandler();

private:

signals:

public slots:
    virtual void handle_games();
    virtual void handle_streams();
    virtual void handle_preview();
    virtual void handle_status(FavoriteItemWidget *item, QNetworkReply* reply);

};

#endif // TWITCHHANDLER_H
