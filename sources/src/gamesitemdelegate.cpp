#include "../include/gamesitemdelegate.h"

GamesItemDelegate::GamesItemDelegate(QObject *parent) :
    QItemDelegate(parent),
    gameFont(QApplication::font()),
    viewersFont(QApplication::font()),
    viewersLabelFont(QApplication::font()),
    viewersLabel("viewers: "),
    channelNrLabel("channels: ")
{
    _state =  QStyle::State_Enabled;

    // font of the game title
    gameFont.setPointSize(12);
    gameFont.setBold(true);

    // font of the viewers
    viewersFont.setPointSize(8);
    viewersFont.setItalic(true);

    // font for the viewersLabel
    viewersLabelFont.setPointSize(8);


}

void GamesItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    painter->save();

    QFontMetrics gamesfm(gameFont);
    QFontMetrics viewersLabelfm(viewersLabelFont);
    QFontMetrics viewersfm(viewersFont);

    QString gameName = qvariant_cast<QString>(index.data(ROLE_NAME));
    QString viewers = qvariant_cast<QString>(index.data(ROLE_VIEWERS));
    QString channels_nr = qvariant_cast<QString>(index.data(ROLE_CHANNEL_NR));
    QString serviceLogoResource = qvariant_cast<QString>(index.data(ROLE_SERVICE_LOGO));

    QSize iconsize = QSize(30,30);

    QRect iconRect = option.rect;
    QRect gameRect = option.rect;
    QRect viewersRect = option.rect;
    QRect viewersLabelRect = option.rect;
    QRect channels_nrRect = option.rect;
    QRect channels_nrLabelRect = option.rect;



    // The rectangle for the serviceIcon
    iconRect.setLeft(iconRect.left()+5);
    iconRect.setRight(iconsize.width()+5);
    iconRect.setTop(iconRect.top()+5);

    // The rectangle for the game name
    gameRect.setLeft(iconRect.right()+5);
    gameRect.setTop(gameRect.top()+5);
    gameRect.setBottom(gameRect.top()+gamesfm.height());

    // The rectangle for the viewersLabel
    viewersLabelRect.setLeft(iconRect.right()+5);
    viewersLabelRect.setRight(viewersLabelRect.left()+viewersLabelfm.width(viewersLabel));
    viewersLabelRect.setTop(gameRect.bottom());

    // The rectangle for the viewers
    viewersRect.setLeft(viewersLabelRect.right());
    viewersRect.setRight(viewersRect.left()+viewersfm.width(viewers));
    viewersRect.setTop(gameRect.bottom());

    // The rectangle for the channelsNrLabel
    channels_nrLabelRect.setLeft(viewersRect.right()+5);
    channels_nrLabelRect.setRight(channels_nrLabelRect.left()+viewersLabelfm.width(channelNrLabel));
    channels_nrLabelRect.setTop(gameRect.bottom());

    // The rectangle for the channels_nr
    channels_nrRect.setLeft(channels_nrLabelRect.right());
    channels_nrRect.setTop(gameRect.bottom());

    // draw the picture
    painter->drawPixmap(iconRect.left(),iconRect.top(),QPixmap(serviceLogoResource).scaled(30,30));

    // Draw game name
    painter->setFont(gameFont);
    painter->drawText(gameRect,gameName);

    // draw the viewers label
    painter->setFont(viewersLabelFont);
    painter->drawText(viewersLabelRect,viewersLabel);

    // number of viewers
    painter->setFont(viewersFont);
    painter->drawText(viewersRect,viewers);

    // channel_nr label
    painter->setFont(viewersLabelFont);
    painter->drawText(channels_nrLabelRect,channelNrLabel);

    // channel_nr
    painter->setFont(viewersFont);
    painter->drawText(channels_nrRect,channels_nr);

    // focus elements
    drawFocus(painter, option, option.rect);

    painter->restore();
}

QSize GamesItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                  const QModelIndex &) const
{

    QSize iconsize = QSize(30,30);
    return(QSize(0,iconsize.height()+12));

}
