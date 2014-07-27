#include "../include/gamesitemdelegate.h"

GamesItemDelegate::GamesItemDelegate(QObject *parent) :
    QItemDelegate(parent),
    headerFont(QApplication::font()),
    legendFont(QApplication::font()),
    infoFont(QApplication::font()),
    viewersLegend("viewers:"),
    channelsLegend("channels:"),
    iconSize(QSize(30,30))
{
    _state =  QStyle::State_Enabled;

    headerFont.setPointSize(12);
    headerFont.setBold(true);

    infoFont.setPointSize(8);
    infoFont.setItalic(true);

    legendFont.setPointSize(8);


}

void GamesItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    painter->save();

    QString gameName = qvariant_cast<QString>(index.data(ROLE_NAME));
    QString viewers = qvariant_cast<QString>(index.data(ROLE_VIEWERS));
    QString channels_nr = qvariant_cast<QString>(index.data(ROLE_CHANNEL_NR));


    QRect iconRect = option.rect;
    QRect gameNameRect = option.rect;
    QRect viewersRect = option.rect;
    QRect viewersLegendRect = option.rect;
    QRect channelsRect = option.rect;
    QRect channelsLegendRect = option.rect;


    printIcon(iconRect, painter, index);


    setRightOf(iconRect,gameNameRect);
    printHeader(gameNameRect,gameName,painter);

    setUnder(gameNameRect, viewersLegendRect);
    printLegend(viewersLegendRect,viewersLegend,painter);

    setRightOf(viewersLegendRect,viewersRect);
    printText(viewersRect,viewers,painter);

    setRightOf(viewersRect,channelsLegendRect);
    printLegend(channelsLegendRect,channelsLegend,painter);

    setRightOf(channelsLegendRect,channelsRect);
    printText(channelsRect,channels_nr,painter);


    // focus elements
    drawFocus(painter, option, option.rect);

    painter->restore();
}

void GamesItemDelegate::printIcon(QRect &iconRect, QPainter* painter, const QModelIndex &index) const
{
    QString serviceLogoResource = qvariant_cast<QString>(index.data(ROLE_SERVICE_LOGO));
    iconRect.setLeft(iconRect.left()+5);
    iconRect.setRight(iconSize.width()+5);
    iconRect.setTop(iconRect.top()+5);
    painter->drawPixmap(iconRect.left(),iconRect.top(),QPixmap(serviceLogoResource).scaled(iconSize));

}

void GamesItemDelegate::setRightOf(QRect &left, QRect &right) const
{
    right.setTop(left.top());
    right.setLeft(left.right()+5);
}

void GamesItemDelegate::setUnder(QRect &upper, QRect &lower) const
{
    lower.setLeft(upper.left());
    lower.setTop(upper.bottom());
}

void GamesItemDelegate::printHeader(QRect &headerRect, QString text, QPainter *painter) const
{
    painter->setFont(headerFont);
    painter->drawText(headerRect,text);
    adjustRectToText(text,headerFont,headerRect);
}

void GamesItemDelegate::printText(QRect &rect, QString text, QPainter *painter) const
{
    painter->setFont(infoFont);
    painter->drawText(rect,text);
    adjustRectToText(text,infoFont,rect);
}

void GamesItemDelegate::printLegend(QRect &rect, QString legend, QPainter *painter) const
{
    painter->setFont(legendFont);
    painter->drawText(rect,legend);
    adjustRectToText(legend,legendFont,rect);
}

void GamesItemDelegate::adjustRectToText(QString msg, QFont font, QRect &rect) const
{
    QFontMetrics fm(font);
    rect.setBottom(rect.top()+fm.height());
    rect.setRight(rect.left()+fm.width(msg));
}

QSize GamesItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                  const QModelIndex &) const
{
    return(QSize(0,iconSize.height()+12));
}
