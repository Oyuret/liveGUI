#include "gamesitemdelegate.h"
#include "networkmanager.h"

GamesItemDelegate::GamesItemDelegate(QObject *parent) :
    QItemDelegate(parent),
    gameFont(QApplication::font()),
    viewersFont(QApplication::font()),
    viewersLabelFont(QApplication::font())
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


    // load the icons
    services.append(QPixmap(":/icons/icons/twitchicon.png").scaled(30,30));
    services.append(QPixmap(":/icons/icons/azubuicon.png").scaled(30,30));

}

void GamesItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    painter->save();

    QFontMetrics gamesfm(gameFont);
    QFontMetrics viewersLabelfm(viewersLabelFont);

    QString gameName = qvariant_cast<QString>(index.data(ROLE_NAME));
    QString viewers = qvariant_cast<QString>(index.data(ROLE_VIEWERS));
    API::SERVICE service = static_cast<API::SERVICE>(index.data(ROLE_SERVICE).toInt());
    QString viewersLabel = "viewers: ";

    QSize iconsize = services.at(service).size();

    QRect gameRect = option.rect;
    QRect viewersRect = option.rect;
    QRect iconRect = option.rect;
    QRect viewersLabelRect = option.rect;

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
    viewersRect.setTop(gameRect.bottom());

    // draw the picture
    painter->drawPixmap(iconRect.left(),iconRect.top(),services.at(service));

    // Draw game name
    painter->setFont(gameFont);
    painter->drawText(gameRect,gameName);

    // draw the viewers label
    painter->setFont(viewersLabelFont);
    painter->drawText(viewersLabelRect,viewersLabel);

    // number of viewers
    painter->setFont(viewersFont);
    painter->drawText(viewersRect,viewers);

    // focus elements
    drawFocus(painter, option, option.rect);

    painter->restore();
}

QSize GamesItemDelegate::sizeHint(const QStyleOptionViewItem &,
                                  const QModelIndex &) const
{
    //QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QSize iconsize = QSize(30,30);
    QFont header_font = QApplication::font();
    header_font.setPixelSize(20);
    QFontMetrics fm(header_font);

    return(QSize(0,iconsize.height()+10));

}
