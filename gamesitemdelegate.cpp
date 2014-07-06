#include "gamesitemdelegate.h"
#include <QStandardItemModel>
#include <QModelIndex>
#include <QStandardItem>
#include <QPainter>
#include <QStyleOptionButton>
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

GamesItemDelegate::GamesItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    _state =  QStyle::State_Enabled;
}

void GamesItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    QString game_name = index.model()->data(index, ROLE_NAME).toString();

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, game_name);
    drawFocus(painter, myOption, myOption.rect);

    QString text2 = index.model()->data(index, ROLE_VIEWERS).toString();

    QStyleOptionViewItem myOption2 = option;
    myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    drawDisplay(painter, myOption2, myOption2.rect, text2);
    drawFocus(painter, myOption2, myOption2.rect);
}

QSize GamesItemDelegate::sizeHint(const QStyleOptionViewItem &/*option*/,
                       const QModelIndex &/*index*/) const
{
    //hard coding size for test purpose,
    //actual size hint can be calculated from option param
    return QSize(100,50);
}
