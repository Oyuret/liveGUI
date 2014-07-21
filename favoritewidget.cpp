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

void FavoriteWidget::add_favorite(const Stream &stream)
{
    if(alreadyInFavorites(stream.getUrl()))
        return;

    QListWidgetItem* favoriteItem = new QListWidgetItem();
    FavoriteItemWidget* favoriteItemWidget = new FavoriteItemWidget(stream, favoriteItem);

    favoriteItem->setSizeHint(favoriteItemWidget->sizeHint());
    connectFavoriteItemWidget(favoriteItemWidget);

    ui->favListWidget->addItem(favoriteItem);
    ui->favListWidget->setItemWidget(favoriteItem,favoriteItemWidget);
}

bool FavoriteWidget::alreadyInFavorites(QString url)
{
    bool alreadyExists = false;

    for(int row=0; row < ui->favListWidget->count(); ++row) {
        QListWidgetItem *favoriteItem = ui->favListWidget->item(row);
        FavoriteItemWidget *favoriteItemWidget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(favoriteItem));

        // compare by url
        if(favoriteItemWidget->getUrl().compare(url)==0) {
            alreadyExists = true;
            break;
        }
    }
    return alreadyExists;
}

void FavoriteWidget::connectFavoriteItemWidget(FavoriteItemWidget* favoriteItemWidget)
{
    QObject::connect(favoriteItemWidget, SIGNAL(go_to_preview()),
                     this,SIGNAL(go_to_preview()));
    QObject::connect(favoriteItemWidget, SIGNAL(fetch_preview(const Stream&)),
                     this,SIGNAL(fetch_preview(const Stream&)));
    QObject::connect(favoriteItemWidget, SIGNAL(play(QString)),
                     this,SIGNAL(play(QString)));
    QObject::connect(favoriteItemWidget, SIGNAL(remove_favorite(QListWidgetItem*)),
                     this,SLOT(remove_favorite(QListWidgetItem*)));
}

void FavoriteWidget::load_favorites()
{
    // store the settings file in the executable's folder
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString numberOfFavorites = settings.value("favsCount", "0").toString();
    QStringList favorites = settings.value("favorites").toStringList();


    for(int row=0; row< numberOfFavorites.toInt(); row++) {
        int index = 5*row;

        int displayNameIndex = index;
        int channelNameIndex = index+1;
        int urlIndex = index+2;
        int serviceIndex = index+3;
        int serviceLogoResourceIndex = index+4;

        QString displayName = favorites.at(displayNameIndex);
        QString channelName = favorites.at(channelNameIndex);
        QString url = favorites.at(urlIndex);
        QString serviceName = favorites.at(serviceIndex);
        QString serviceLogoResource = favorites.at(serviceLogoResourceIndex);

        add_favorite(FavoriteStream(displayName,channelName,url,serviceName, serviceLogoResource));
    }
}

void FavoriteWidget::save_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString numberOfFavorites = QString::number(ui->favListWidget->count());
    QStringList favorites;

    // fetch status for each item
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));
        Stream stream = widget->getStream();
        favorites << stream.getDisplayName();
        favorites << stream.getChannelName();
        favorites << stream.getUrl();
        favorites << stream.getServiceName();
        favorites << stream.getServiceLogoResource();
    }

    settings.setValue("favsCount", numberOfFavorites);
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
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));

        // (UGLY) We set the removeButton on the itemWidget disabled so the pointer will be live
        // when the network request is done
        widget->set_button_disabled();
        widget->set_checking();
        emit fetch_status(widget->getStream(), widget);
    }
}


FavoriteWidget::FavoriteStream::FavoriteStream(QString displayName, QString channelName, QString url, QString serviceName, QString logo)
{
    this->displayName = displayName;
    this->channelName = channelName;
    this->url = url;
    this->serviceName = serviceName;
    this->serviceLogoResource = logo;
}
