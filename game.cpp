#include "game.h"

Game::Game()
{
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

API::SERVICE Game::getService() const
{
    return service;
}
