#ifndef GAME_H
#define GAME_H
#include <QJsonObject>
#include "global.h"
#include <QDebug>

class Game
{
public:
    Game();
    virtual ~Game() {}
    virtual void read(QJsonObject) {}

    QString getName() const;
    QString getViewers() const;
    QString getChannels() const;
    API::SERVICE getService() const;

protected:
    QString name;
    QString viewers;
    QString channels;
    API::SERVICE service;
};

#endif // GAME_H
