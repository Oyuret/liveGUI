#ifndef AZUBUHANDLER_H
#define AZUBUHANDLER_H

#include <QObject>
#include "abstracthandler.h"
#include "game.h"
#include "stream.h"

class AzubuHandler : public AbstractHandler
{
    Q_OBJECT
public:
    explicit AzubuHandler();
    virtual QUrl getGamesUrl() const;
    virtual QUrl getStreamsUrl(QString game) const;
    virtual QUrl getPreviewUrl(QString channelName) const;
    virtual QUrl getStatusUrl(QString channelName) const;

    static QString getServiceName() { return "azubu"; }
    static QString getBaseUrl() {return "http://www.azubu.tv/";}

private:

    class AzubuGame : public Game {
    public:
        AzubuGame() : Game() {}
        virtual ~AzubuGame() {}
        virtual void read(QJsonObject gameJson);
    };

    class AzubuStream : public Stream {
    public:
        AzubuStream() : Stream() {}
        virtual ~AzubuStream() {}
        virtual void read(QJsonObject streamJson);
    };

    QVector<QUrl> urls {
                QUrl("http://liveleaguestream.com/json.php?method=getAll"),
                QUrl("http://liveleaguestream.com/json.php?method=getOnline"),
                QUrl("http://liveleaguestream.com/json.php?method=getPlayer&player="),
                QUrl("http://liveleaguestream.com/json.php?method=getPlayer&player=")
    };

signals:

public slots:
    virtual void handleGames();
    virtual void handleStreams();
    virtual void handlePreview();
    virtual void handleStatus(const Stream& stream, QNetworkReply* reply);

};

#endif // AZUBUHANDLER_H
