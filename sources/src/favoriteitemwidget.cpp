#include "../include/favoriteitemwidget.h"
#include "ui_favoriteitemwidget.h"

FavoriteItemWidget::FavoriteItemWidget(const Stream& stream, QListWidgetItem *item) :
    QWidget(0),
    ui(new Ui::FavoriteItemWidget),
    item(item),
    stream(stream)
{
    ui->setupUi(this);
    ui->streamerName->setText(stream.getDisplayName());
    ui->urlText->setText(stream.getUrl());

    QPixmap icon(stream.getServiceLogoResource());
    ui->serviceIcon->setPixmap(icon);
}

FavoriteItemWidget::~FavoriteItemWidget()
{
    delete ui;
}

Stream FavoriteItemWidget::getStream() const
{
    return stream;
}

void FavoriteItemWidget::setOnline()
{
    QPixmap icon(":/icons/icons/online.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::setOffline()
{
    QPixmap icon(":/icons/icons/offline.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::setChecking()
{
    QPixmap icon(":/icons/icons/checking-status.png");
    ui->onlineStatus->setPixmap(icon);
}

void FavoriteItemWidget::on_playStreamButton_clicked()
{
    emit play(stream.getUrl());
}

void FavoriteItemWidget::on_previewStreamButton_clicked()
{
    emit goToPreview();
    emit fetchStreamPreview(stream);
}

void FavoriteItemWidget::on_removeFavoriteButton_clicked()
{
    emit removeFavorite(item);
}
