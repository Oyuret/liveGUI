#ifndef ABSTRACTHANDLER_H
#define ABSTRACTHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <QUrl>
#include <QList>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrlQuery>
#include <QMessageBox>
#include "global.h"
#include "favoriteitemwidget.h"

class AbstractHandler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractHandler(QObject *parent = 0);
    virtual ~AbstractHandler(){}

signals:
    // signal main window
    void add_game(QString name, QString viewers, QString nr_of_chans, API::SERVICE service);
    void add_stream(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service);

    // signal preview
    void set_preview(QString streamer, QString game, QString viewers,
                     QString previewUrl, QString status, QString delay,
                     QString logoUrl, QString url, API::SERVICE service);
    void reset_preview();

public slots:
    virtual void handle_games(){}
    virtual void handle_streams(){}
    virtual void handle_preview(){}
    virtual void handle_status(FavoriteItemWidget*, QNetworkReply*){}
    virtual void handle_status_error(FavoriteItemWidget*){}

};

#endif // ABSTRACTHANDLER_H
