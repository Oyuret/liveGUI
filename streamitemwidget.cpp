#include "streamitemwidget.h"
#include "ui_streamitemwidget.h"

StreamItemWidget::StreamItemWidget(QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service) :
    QWidget(),
    ui(new Ui::StreamItemWidget)
{
    ui->setupUi(this);
    ui->streamerName->setText(name);
    ui->statusText->setText(status);
    ui->gameText->setText(game);
    ui->viewersText->setText(viewers);
    ui->urlText->setText(url);

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
