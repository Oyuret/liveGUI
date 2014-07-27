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

enum Roles {
    ROLE_NAME = Qt::UserRole+1,
    ROLE_VIEWERS = Qt::UserRole+2,
    ROLE_CHANNEL_NR = Qt::UserRole+3,
    ROLE_SERVICE_NAME = Qt::UserRole+4,
    ROLE_SERVICE_LOGO = Qt::UserRole+5
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

private:
    void printIcon(QRect& iconRect, QPainter *painter, const QModelIndex& index) const;
    void printHeader(QRect& headerRect, QString text, QPainter* painter) const;
    void printText(QRect& rect, QString text, QPainter* painter) const;
    void printLegend(QRect& rect, QString legend, QPainter* painter) const;
    void adjustRectToText(QString msg, QFont fm, QRect &rect) const;
    void setRightOf(QRect& left, QRect& right) const;
    void setUnder(QRect& upper, QRect& lower) const;

    QStyle::State  _state;

    QFont headerFont;
    QFont legendFont;
    QFont infoFont;

    QString viewersLegend;
    QString channelsLegend;

    QSize iconSize;

};

#endif // GAMESITEMDELEGATE_H
