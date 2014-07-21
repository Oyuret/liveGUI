#include "../include/livestream.h"

LiveStream::LiveStream(QObject *parent) :
    QProcess(parent)
{
}

void LiveStream::play(QString adress, QString quality) {
    QString program = "livestreamer";
    QStringList arguments;
    arguments << adress << quality;
    start(program, arguments);
}
