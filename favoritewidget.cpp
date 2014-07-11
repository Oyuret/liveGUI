#include "favoritewidget.h"
#include "ui_favoritewidget.h"
#include <QDebug>

FavoriteWidget::FavoriteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteWidget)
{
    ui->setupUi(this);
}

FavoriteWidget::~FavoriteWidget()
{
    delete ui;
}

void FavoriteWidget::add_favorite(QString displayName, QString channelName, QString url, API::SERVICE service)
{
    // Check if we already have the item
    for(int i=0; i< ui->favListWidget->count(); ++i) {
        QListWidgetItem *favoriteItem = ui->favListWidget->item(i);
        FavoriteItemWidget* favoriteItemWidget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(favoriteItem));

        if(favoriteItemWidget->url.compare(url)==0) {
            return;
        }
    }


    QListWidgetItem* favoriteItem = new QListWidgetItem();
    FavoriteItemWidget* favoriteItemWidget = new FavoriteItemWidget(displayName, channelName, url, favoriteItem, service);
    favoriteItem->setSizeHint(favoriteItemWidget->sizeHint());
    ui->favListWidget->addItem(favoriteItem);



    QObject::connect(favoriteItemWidget, SIGNAL(fetch_preview(QString,API::SERVICE)),this,SIGNAL(fetch_preview(QString,API::SERVICE)));
    QObject::connect(favoriteItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));
    QObject::connect(favoriteItemWidget, SIGNAL(remove_favorite(QListWidgetItem*)),this,SLOT(remove_favorite(QListWidgetItem*)));
    ui->favListWidget->setItemWidget(favoriteItem,favoriteItemWidget);
}

void FavoriteWidget::load_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString numberOfFavorites = settings.value("favsCount", "0").toString();
    QStringList favorites = settings.value("favorites").toStringList();


    for(int i=0; i< numberOfFavorites.toInt(); i++) {
        int index = 4*i;

        int displayNameIndex = index;
        int channelNameIndex = index+1;
        int urlIndex = index+2;
        int serviceIndex = index+3;

        QString displayName = favorites.at(displayNameIndex);
        QString channelName = favorites.at(channelNameIndex);
        QString url = favorites.at(urlIndex);
        API::SERVICE service = static_cast<API::SERVICE>(favorites.at(serviceIndex).toInt());

        add_favorite(displayName,channelName,url,service);
    }
}

void FavoriteWidget::save_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString favsCount = QString::number(ui->favListWidget->count());
    QStringList favorites;

    // fetch status for each item
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));
        favorites << widget->streamer_name;
        favorites << widget->name;
        favorites << widget->url;
        favorites << QString::number(widget->service);
    }

    settings.setValue("favsCount", favsCount);
    settings.setValue("favorites", favorites);
}

void FavoriteWidget::remove_favorite(QListWidgetItem *item)
{
    int row = ui->favListWidget->row(item);
    QListWidgetItem* removed = ui->favListWidget->takeItem(row);
    delete removed;
}

void FavoriteWidget::on_refreshStatusButton_clicked()
{
    // fetch status for each item
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));
        widget->set_button_disabled();
        emit fetch_status(widget->name, widget->service, widget);
    }
}
