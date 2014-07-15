#ifndef GLOBAL_H
#define GLOBAL_H
#include <QVector>

namespace API {

static QVector<QString> logos = {
    QString(":/icons/icons/twitchicon.png"),
    QString(":/icons/icons/azubuicon.png")
};


enum SERVICE {
    TWITCH,
    AZUBU,
    SERVICE_END
};

enum REQUEST_TYPE {
    GAMES,
    STREAMS,
    PREVIEW,
    STATUS,
    REQUEST_TYPE_END
};

}

#endif // GLOBAL_H
