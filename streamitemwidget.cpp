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

    switch(service) {
    case API::TWITCH:
        QPixmap icon(":/icons/icons/twitchicon.png");
        ui->serviceIcon->setPixmap(icon);
        break;
    }


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
    emit preview(this->name, this->service);
}
