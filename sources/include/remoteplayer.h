#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

class RemotePlayer : public QObject
{
    Q_OBJECT
public:
    RemotePlayer();

    void play(QString streamUrl, QString remoteUrl);
    void stop(QString remoteUrl);

private:
    QNetworkAccessManager remote;
    QNetworkRequest createNetworkRequest(QUrl requestUrl) const;
    void connectReply(QNetworkReply *reply);

private slots:
    void slotError(QNetworkReply::NetworkError);
    void handleResponse();
};

#endif // REMOTEPLAYER_H
