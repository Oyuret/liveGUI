#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <QUrl>
#include <QList>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrlQuery>

namespace API {

enum SERVICE {
    TWITCH,
    AZUBU
};

enum TYPE {
    GAMES,
    STREAMS,
    PREVIEW
};

}

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT

private:
    QVector<QVector<QUrl>> urls;

public:
    explicit NetworkManager(QObject *parent = 0);

signals:

    // signal main window
    void add_game(QString name, QString viewers, API::SERVICE service);
    void add_stream(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service);

    // signal preview
    void set_preview(QString streamer, QString game, QString viewers,
                     QString previewUrl, QString status, QString delay,
                     QString logoUrl, API::SERVICE service);
    void reset_preview();


public slots:
    void fetch_games(API::SERVICE service);
    void fetch_streams_by_game(QString game, API::SERVICE service);
    void fetch_preview(QString name, API::SERVICE service);

private slots:

    // Twitch
    void handle_twitch_games();
    void handle_twitch_streams();
    void handle_twitch_preview();


};

#endif // NETWORKMANAGER_H
