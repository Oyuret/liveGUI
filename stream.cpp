#include "stream.h"

Stream::Stream()
{
}
QString Stream::getDisplayName() const
{
    return displayName;
}

QString Stream::getChannelName() const
{
    return channelName;
}

QString Stream::getStatus() const
{
    return status;
}

QString Stream::getGame() const
{
    return game;
}

QString Stream::getViewers() const
{
    return viewers;
}

QString Stream::getUrl() const
{
    return url;
}

QString Stream::getPreviewUrl() const
{
    return previewUrl;
}

QString Stream::getLogoUrl() const
{
    return logoUrl;
}

QString Stream::getDelay() const
{
    return delay;
}
API::SERVICE Stream::getService() const
{
    return service;
}











