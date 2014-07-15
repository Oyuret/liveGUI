#include "streamitemwidget.h"
#include "ui_streamitemwidget.h"

StreamItemWidget::StreamItemWidget(std::shared_ptr<Stream> stream) :
    QWidget(),
    ui(new Ui::StreamItemWidget),
    stream(stream)
{
    ui->setupUi(this);
    ui->streamerName->setText(stream->getDisplayName());
    ui->statusText->setText(stream->getStatus());
    ui->gameText->setText(stream->getGame());
    ui->viewersText->setText(stream->getViewers());
    ui->urlText->setText(stream->getUrl());

    QPixmap icon(API::logos.at(stream->getService()));
    ui->serviceIcon->setPixmap(icon);
}

StreamItemWidget::~StreamItemWidget()
{
    delete ui;
}

void StreamItemWidget::on_playStreamButton_clicked()
{
    emit play(stream->getUrl());
}

void StreamItemWidget::on_previewStreamButton_clicked()
{
    emit go_to_preview();
    emit fetch_preview(stream->getChannelName(), stream->getService());
}

void StreamItemWidget::on_favoriteButton_clicked()
{
    emit add_favorite(stream->getDisplayName(),stream->getChannelName(),
                      stream->getUrl(),stream->getService());
}
