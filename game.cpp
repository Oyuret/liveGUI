#include "game.h"

Game::Game() {

}

Game::Game(QString name, QString serviceName)
{
    this->name = name;
    this->serviceName = serviceName;
}
QString Game::getName() const
{
    return name;
}
QString Game::getViewers() const
{
    return viewers;
}

QString Game::getChannels() const
{
    return channels;
}
