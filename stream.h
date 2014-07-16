#ifndef STREAM_H
#define STREAM_H
#include <QJsonObject>
#include <QDebug>
#include "global.h"

class Stream
{
public:
    Stream();
    virtual ~Stream() {}

    virtual void read(QJsonObject) {}

    QString getDisplayName() const;
    QString getChannelName() const;
    QString getStatus() const;
    QString getGame() const;
    QString getViewers() const;
    QString getUrl() const;
    QString getPreviewUrl() const;
    QString getLogoUrl() const;
    QString getDelay() const;

    API::SERVICE getService() const;

protected:
    QString displayName;
    QString channelName;
    QString status;
    QString game;
    QString viewers;
    QString url;

    QString previewUrl;
    QString logoUrl;
    QString delay;

    API::SERVICE service;

};

#endif // STREAM_H
