#include "favoriteitemwidget.h"
#include "ui_favoriteitemwidget.h"
#include <QDebug>

FavoriteItemWidget::FavoriteItemWidget(QString streamerName, QString name, QString url, QListWidgetItem *item, API::SERVICE service) :
    QWidget(0),
    ui(new Ui::FavoriteItemWidget),
    item(item),
    name(name),
    service(service)
{
    ui->setupUi(this);
    ui->streamerName->setText(streamerName);
    ui->urlText->setText(url);

    QString service_name;

    switch(service) {
    case API::TWITCH:
        service_name.append(":/icons/icons/twitchicon.png");
        break;
    case API::AZUBU:
        break;
    }

    QPixmap icon(service_name);
    ui->serviceIcon->setPixmap(icon);
}

FavoriteItemWidget::~FavoriteItemWidget()
{
    delete ui;
}

void FavoriteItemWidget::set_online()
{
    QPixmap icon(":/icons/icons/online.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::set_offline()
{
    QPixmap icon(":/icons/icons/offline.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::set_checking()
{
    QPixmap icon(":/icons/icons/checking-status.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::on_playStreamButton_clicked()
{
    emit play(ui->urlText->text());
}

void FavoriteItemWidget::on_previewStreamButton_clicked()
{
    emit preview(name,service);
}

void FavoriteItemWidget::on_removeFavoriteButton_clicked()
{
    emit remove_favorite(item);
}
