#ifndef GLOBAL_H
#define GLOBAL_H

namespace API {

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
