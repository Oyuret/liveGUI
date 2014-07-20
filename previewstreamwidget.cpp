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

void PreviewStreamWidget::set_preview(const Stream& stream)
{
    this->stream = stream;

    ui->streamerLabel->setText(stream.getDisplayName());
    ui->gameLabel->setText(stream.getGame());
    ui->viewersLabel->setText(stream.getViewers());
    ui->statusLabel->setText(stream.getStatus());
    ui->delayLabel->setText(stream.getDelay());

    QPixmap icon(API::logos.at(stream.getService()));
    ui->serviceIcon->setPixmap(icon);

    getPreviewImage(stream);
    getStreamLogo(stream);
}

void PreviewStreamWidget::getPreviewImage(const Stream& stream)
{
    QNetworkRequest requestPreviewImage;
    requestPreviewImage.setUrl(QUrl(stream.getPreviewUrl()));
    requestPreviewImage.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *replyPreviewImage = imageNetwork.get(requestPreviewImage);
    connect(replyPreviewImage, SIGNAL(finished()), this, SLOT(handle_preview()));
}

void PreviewStreamWidget::getStreamLogo(const Stream& stream)
{
    QNetworkRequest requestLogo;
    requestLogo.setUrl(QUrl(stream.getLogoUrl()));
    requestLogo.setPriority(QNetworkRequest::HighPriority);

    QNetworkReply *replyLogo = imageNetwork.get(requestLogo);
    connect(replyLogo, SIGNAL(finished()), this, SLOT(handle_logo()));
}

void PreviewStreamWidget::reset_preview()
{
    stream = Stream();
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
        reply->deleteLater();
        return;
    }

    QByteArray jpegData = reply->readAll();
    bool sucess = pixmap.loadFromData(jpegData);

    if(!sucess) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->streamerIcon->setPixmap(icon);
        reply->deleteLater();
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
        reply->deleteLater();
        return;
    }

    QByteArray jpegData = reply->readAll();
    bool sucess = pixmap.loadFromData(jpegData);

    if(!sucess) {
        QPixmap icon(":/icons/icons/missing.png");
        ui->previewIcon->setPixmap(icon);
        reply->deleteLater();
        return;
    }


    ui->previewIcon->setPixmap(pixmap);

    reply->deleteLater();
}

void PreviewStreamWidget::on_playButton_clicked()
{
    if(!stream.getUrl().isEmpty()) emit play(stream.getUrl());
}

void PreviewStreamWidget::on_favoriteButton_clicked()
{
    if(!stream.getUrl().isEmpty()) emit add_favorite(stream.getDisplayName(),stream.getChannelName(),
                                 stream.getUrl(), stream.getService());
}
