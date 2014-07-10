#include "streamitemwidget.h"
#include "ui_streamitemwidget.h"

StreamItemWidget::StreamItemWidget(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service) :
    QWidget(),
    ui(new Ui::StreamItemWidget)
{
    ui->setupUi(this);
    ui->streamerName->setText(streamer);
    ui->statusText->setText(status);
    ui->gameText->setText(game);
    ui->viewersText->setText(viewers);
    ui->urlText->setText(url);
    this->service = service;
    this->name = name;

    QString iconResource;

    switch(service) {
    case API::TWITCH:
        iconResource.append(":/icons/icons/twitchicon.png");
        break;
    case API::AZUBU:
        iconResource.append(":/icons/icons/azubuicon.png");
        break;
    }

    QPixmap icon(iconResource);
    ui->serviceIcon->setPixmap(icon);


}

StreamItemWidget::~StreamItemWidget()
{
    delete ui;
}

void StreamItemWidget::on_playStreamButton_clicked()
{
    emit play(ui->urlText->text());
}

void StreamItemWidget::on_previewStreamButton_clicked()
{
    emit fetch_preview(this->name, this->service);
}

void StreamItemWidget::on_favoriteButton_clicked()
{
    emit add_favorite(ui->streamerName->text(),name,ui->urlText->text(),service);
}
