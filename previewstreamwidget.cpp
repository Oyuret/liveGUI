#include "previewstreamwidget.h"
#include "ui_previewstreamwidget.h"

PreviewStreamWidget::PreviewStreamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewStreamWidget)
{
    ui->setupUi(this);
    reset_preview();
}

PreviewStreamWidget::~PreviewStreamWidget()
{
    delete ui;
}

void PreviewStreamWidget::set_preview(QString streamer, QString game, QString viewers,
                                      QString previewUrl, QString status, QString delay,
                                      QString logoUrl, QString url, API::SERVICE service)
{

    ui->streamerLabel->setText(streamer);
    ui->gameLabel->setText(game);
    ui->viewersLabel->setText(viewers);
    ui->statusLabel->setText(status);
    ui->delayLabel->setText(delay);

    this->url.clear();
    this->url.append(url);

    this->service = service;

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

    // get preview
    QNetworkRequest requestPreview;
    requestPreview.setUrl(QUrl(previewUrl));
    requestPreview.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *replyPreview = network.get(requestPreview);
    connect(replyPreview, SIGNAL(finished()), this, SLOT(handle_preview()));

    // get preview
    QNetworkRequest requestLogo;
    requestLogo.setUrl(QUrl(logoUrl));
    requestLogo.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *replyLogo = network.get(requestLogo);
    connect(replyLogo, SIGNAL(finished()), this, SLOT(handle_logo()));


}

void PreviewStreamWidget::reset_preview()
{
    ui->streamerLabel->setText("Offline");
    ui->gameLabel->setText("GameOver");
    ui->viewersLabel->setText("None");
    ui->statusLabel->setText("Offline");
    ui->delayLabel->setText("Inf");

    QPixmap icon(":/icons/icons/missing.png");
    ui->serviceIcon->setPixmap(icon);
    ui->previewIcon->setPixmap(icon);
    ui->streamerIcon->setPixmap(icon);
}

void PreviewStreamWidget::handle_logo()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QPixmap pixmap;

    if (reply->error() != QNetworkReply::NoError) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->streamerIcon->setPixmap(icon);
        return;
    }

    QByteArray jpegData = reply->readAll();
    bool sucess = pixmap.loadFromData(jpegData);

    if(!sucess) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->streamerIcon->setPixmap(icon);
        return;
    }


    ui->streamerIcon->setPixmap(pixmap);

    reply->deleteLater();
}

void PreviewStreamWidget::handle_preview()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QPixmap pixmap;

    if (reply->error() != QNetworkReply::NoError) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->previewIcon->setPixmap(icon);
        return;
    }

    QByteArray jpegData = reply->readAll();
    bool sucess = pixmap.loadFromData(jpegData);

    if(!sucess) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->previewIcon->setPixmap(icon);
        return;
    }


    ui->previewIcon->setPixmap(pixmap);

    reply->deleteLater();
}

void PreviewStreamWidget::on_playButton_clicked()
{
    if(!url.isEmpty()) emit play(url);
}

void PreviewStreamWidget::on_favoriteButton_clicked()
{
    // fulhack
    QString name = url.split("/").last();
    if(!url.isEmpty()) emit add_favorite(ui->streamerLabel->text(),name, url, service);
}
