#ifndef LIVESTREAM_H
#define LIVESTREAM_H

#include <QProcess>
#include <QObject>

class LiveStream : public QProcess
{
    Q_OBJECT

public:
    explicit LiveStream(QObject *parent = 0);

signals:

public slots:
    void play(QString adress, QString quality);

};

#endif // LIVESTREAM_H
