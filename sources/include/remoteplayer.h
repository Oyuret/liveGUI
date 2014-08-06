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

    void play(QString url);
    void stop();

private:
    QNetworkAccessManager remote;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void handleResponse();
};

#endif // REMOTEPLAYER_H
