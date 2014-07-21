#include "favoriteitemwidget.h"
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

QString FavoriteItemWidget::getUrl() const
{
    return stream.getUrl();
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

void FavoriteItemWidget::set_button_enabled()
{
    ui->removeFavoriteButton->setEnabled(true);
}

void FavoriteItemWidget::set_button_disabled()
{
    ui->removeFavoriteButton->setEnabled(false);
}

void FavoriteItemWidget::on_playStreamButton_clicked()
{
    emit play(stream.getUrl());
}

void FavoriteItemWidget::on_previewStreamButton_clicked()
{
    emit goToPreview();
    emit fetch_preview(stream);
}

void FavoriteItemWidget::on_removeFavoriteButton_clicked()
{
    emit removeFavorite(item);
}
