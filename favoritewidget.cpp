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

void FavoriteWidget::add_favorite(QString streamerName, QString name, QString url, API::SERVICE service)
{
    // Check if we already have the item
    for(int i=0; i< ui->favListWidget->count(); ++i) {
        QListWidgetItem *item = ui->favListWidget->item(i);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));

        if(widget->url.compare(url)==0) {
            return;
        }
    }


    QListWidgetItem* item = new QListWidgetItem();
    FavoriteItemWidget* widget = new FavoriteItemWidget(streamerName, name, url, item, service);
    item->setSizeHint(widget->sizeHint());
    ui->favListWidget->addItem(item);



    QObject::connect(widget, SIGNAL(fetch_preview(QString,API::SERVICE)),this,SIGNAL(fetch_preview(QString,API::SERVICE)));
    QObject::connect(widget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));
    QObject::connect(widget, SIGNAL(remove_favorite(QListWidgetItem*)),this,SLOT(remove_favorite(QListWidgetItem*)));
    ui->favListWidget->setItemWidget(item,widget);
}

void FavoriteWidget::load_favorites()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString favsCountValue = settings.value("favsCount", "0").toString();
    QStringList favorites = settings.value("favorites").toStringList();


    for(int i=0; i< favsCountValue.toInt(); i++) {
        int index = 4*i;
        QString streamer_name = favorites.at(index);
        QString name = favorites.at(index+1);
        QString url = favorites.at(index+2);
        API::SERVICE service = static_cast<API::SERVICE>(favorites.at(index+3).toInt());

        add_favorite(streamer_name,name,url,service);
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
