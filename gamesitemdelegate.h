#ifndef GAMESITEMDELEGATE_H
#define GAMESITEMDELEGATE_H

#include <QItemDelegate>

enum Roles {
  ROLE_NAME = Qt::UserRole+1,
  ROLE_VIEWERS = Qt::UserRole+2,
  ROLE_TITLE = Qt::UserRole+3,
  ROLE_URL = Qt::UserRole+4,
  ROLE_SERVICE = Qt::UserRole+5
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

signals:

public slots:

};

#endif // GAMESITEMDELEGATE_H
