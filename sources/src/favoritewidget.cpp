#include "../include/favoritewidget.h"
#include "ui_favoritewidget.h"

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

bool FavoriteWidget::alreadyInFavorites(QString url) const
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
    QObject::connect(favoriteItemWidget, SIGNAL(goToPreview()),
                     this,SIGNAL(goToPreview()));
    QObject::connect(favoriteItemWidget, SIGNAL(fetch_preview(const Stream&)),
                     this,SIGNAL(fetch_preview(const Stream&)));
    QObject::connect(favoriteItemWidget, SIGNAL(play(QString)),
                     this,SIGNAL(play(QString)));
    QObject::connect(favoriteItemWidget, SIGNAL(removeFavorite(QListWidgetItem*)),
                     this,SLOT(removeFavorite(QListWidgetItem*)));
}

void FavoriteWidget::load_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    int numberOfFavorites = settings.beginReadArray("favorites");

    for(int i=0; i < numberOfFavorites; ++i) {
        settings.setArrayIndex(i);

        QString displayName = settings.value("displayName").toString();
        QString channelName = settings.value("channelName").toString();
        QString url = settings.value("url").toString();
        QString serviceName = settings.value("serviceName").toString();
        QString serviceLogoResource = settings.value("serviceLogoResource").toString();

        FavoriteStream favorite(displayName,channelName,url,serviceName,serviceLogoResource);
        add_favorite(favorite);
    }

    settings.endArray();
}

void FavoriteWidget::save_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    settings.beginWriteArray("favorites");

    for(int row=0; row < ui->favListWidget->count(); ++row) {
        settings.setArrayIndex(row);

        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));
        Stream stream = widget->getStream();

        settings.setValue("displayName", stream.getDisplayName());
        settings.setValue("channelName", stream.getChannelName());
        settings.setValue("url", stream.getUrl());
        settings.setValue("serviceName", stream.getServiceName());
        settings.setValue("serviceLogoResource", stream.getServiceLogoResource());
    }

    settings.endArray();
}

void FavoriteWidget::removeFavorite(QListWidgetItem *item)
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
