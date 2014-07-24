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

protected:
    enum REQUEST_TYPE {
        GAMES,
        STREAMS,
        PREVIEW,
        STATUS
    };

signals:
    // signal main window
    void addGame(const Game& game);
    void addStream(const Stream& stream);

    // signal preview
    void setPreview(const Stream& stream);
    void resetPreview();

    // fetch next
    void fetch_next_games(QString url, const Service& service);

    // stream status
    void streamOnline(const Stream& stream);
    void streamOffline(const Stream& stream);
    void streamUncertain(const Stream& stream);

public slots:
    virtual void handleGames() = 0;
    virtual void handleStreams() = 0;
    virtual void handlePreview() = 0;
    virtual void handleStatus(const Stream& stream, QNetworkReply*) = 0;

};

#endif // ABSTRACTHANDLER_H
