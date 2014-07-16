#ifndef ABSTRACTHANDLER_H
#define ABSTRACTHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <QUrl>
#include <QUrlQuery>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "global.h"
#include "favoriteitemwidget.h"
#include "game.h"
#include "stream.h"

class AbstractHandler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractHandler(QObject *parent = 0);
    virtual ~AbstractHandler(){}

    virtual QUrl getGamesUrl() const = 0;
    virtual QUrl getStreamsUrl(QString game) const = 0;
    virtual QUrl getPreviewUrl(QString channelName) const = 0;
    virtual QUrl getStatusUrl(QString channelName) const = 0;

signals:
    // signal main window
    void add_game(const Game& game);
    void add_stream(const Stream& stream);

    // signal preview
    void set_preview(const Stream& stream);
    void reset_preview();

    // fetch next
    void fetch_next_games(QString url, API::SERVICE service);

public slots:
    virtual void handle_games() = 0;
    virtual void handle_streams() = 0;
    virtual void handle_preview() = 0;
    virtual void handle_status(FavoriteItemWidget*, QNetworkReply*) = 0;

};

#endif // ABSTRACTHANDLER_H
