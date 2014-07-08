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
    painter->save();

    QFont font = QApplication::font();
    QFont SubFont = QApplication::font();
    font.setPixelSize(20);
    font.setBold(true);
    SubFont.setWeight(SubFont.weight()-2);
    QFontMetrics fm(font);

    QIcon icon(":/icons/icons/favorite.png");
    QString headerText = qvariant_cast<QString>(index.data(ROLE_NAME));
    QString subText = qvariant_cast<QString>(index.data(ROLE_VIEWERS));

    QSize iconsize = QSize(20,20);

    QRect headerRect = option.rect;
    QRect subheaderRect = option.rect;
    QRect iconRect = subheaderRect;

    iconRect.setRight(iconsize.width()+30);
    iconRect.setTop(iconRect.top()+5);
    headerRect.setLeft(iconRect.right());
    subheaderRect.setLeft(iconRect.right());
    headerRect.setTop(headerRect.top()+5);
    headerRect.setBottom(headerRect.top()+fm.height());

    subheaderRect.setTop(headerRect.bottom()+2);


    //painter->drawPixmap(QPoint(iconRect.right()/2,iconRect.top()/2),icon.pixmap(iconsize.width(),iconsize.height()));
    painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2+2,iconRect.top()+iconsize.height()/2+3),icon.pixmap(iconsize.width(),iconsize.height()));

    painter->setFont(font);
    painter->drawText(headerRect,headerText);


    painter->setFont(SubFont);
    painter->drawText(subheaderRect.left(),subheaderRect.top()+17,subText);

    drawFocus(painter, option, option.rect);

    painter->restore();
}

QSize GamesItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                  const QModelIndex &) const
{
    //QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QSize iconsize = QSize(20,20);
    QFont header_font = QApplication::font();
    header_font.setPixelSize(20);
    QFontMetrics fm(header_font);

    return(QSize(iconsize.width(),iconsize.height()+fm.height() + 12 ));

}
