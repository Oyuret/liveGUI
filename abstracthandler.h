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
#include <memory>
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

signals:
    // signal main window
    void add_game(std::shared_ptr<Game> game);
    void add_stream(std::shared_ptr<Stream> stream);

    // signal preview
    void set_preview(std::shared_ptr<Stream> stream);
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
