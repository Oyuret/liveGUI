#include "streamitemwidget.h"
#include "ui_streamitemwidget.h"

StreamItemWidget::StreamItemWidget(const Stream &stream) :
    QWidget(),
    ui(new Ui::StreamItemWidget),
    stream(stream)
{
    ui->setupUi(this);
    ui->streamerName->setText(stream.getDisplayName());
    ui->statusText->setText(stream.getStatus());
    ui->gameText->setText(stream.getGame());
    ui->viewersText->setText(stream.getViewers());
    ui->urlText->setText(stream.getUrl());

    QPixmap icon(stream.getServiceLogoResource());
    ui->serviceIcon->setPixmap(icon);
}

StreamItemWidget::~StreamItemWidget()
{
    delete ui;
}

void StreamItemWidget::on_playStreamButton_clicked()
{
    emit play(stream.getUrl());
}

void StreamItemWidget::on_previewStreamButton_clicked()
{
    emit goToPreview();
    emit fetch_preview(stream);
}

void StreamItemWidget::on_favoriteButton_clicked()
{
    emit add_favorite(stream);
}
