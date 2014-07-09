#ifndef GAMESITEMDELEGATE_H
#define GAMESITEMDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QStandardItem>
#include <QPainter>
#include <QStyleOptionButton>
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include "global.h"

enum Roles {
  ROLE_NAME = Qt::UserRole+1,
  ROLE_VIEWERS = Qt::UserRole+2,
  ROLE_SERVICE = Qt::UserRole+3,
  ROLE_CHANNEL_NR = Qt::UserRole+4
};

class GamesItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    GamesItemDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const ;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const ;

signals:

private:
    QStyle::State  _state;

    // infomatics fonts
    QFont gameFont;
    QFont viewersFont;

    // label fonts
    QFont viewersLabelFont;

    // icons for services
    QVector<QPixmap> services;

    // static labels
    QString viewersLabel;
    QString channelNrLabel;


signals:

public slots:

};

#endif // GAMESITEMDELEGATE_H
