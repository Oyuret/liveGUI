#ifndef GAME_H
#define GAME_H
#include <QJsonObject>
#include "service.h"
#include <QDebug>

class Game : public Service
{
public:
    Game();
    Game(QString name, QString serviceName);
    virtual ~Game() {}
    virtual void read(QJsonObject) {}

    QString getName() const;
    QString getViewers() const;
    QString getChannels() const;

protected:
    QString name;
    QString viewers;
    QString channels;
};

#endif // GAME_H
