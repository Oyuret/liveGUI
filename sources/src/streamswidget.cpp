#include "../include/streamswidget.h"
#include "ui_streamswidget.h"

StreamsWidget::StreamsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamsWidget)
{
    ui->setupUi(this);
}

StreamsWidget::~StreamsWidget()
{
    delete ui;
}

void StreamsWidget::add_stream(const Stream &stream)
{
    QListWidgetItem* streamItem = new QListWidgetItem();
    StreamItemWidget* streamItemWidget = new StreamItemWidget(stream);

    connectStreamItemWidget(streamItemWidget);
    streamItem->setSizeHint(streamItemWidget->sizeHint());

    ui->streamListWidget->addItem(streamItem);
    ui->streamListWidget->setItemWidget(streamItem,streamItemWidget);
}

void StreamsWidget::connectStreamItemWidget(StreamItemWidget* streamItemWidget)
{
    QObject::connect(streamItemWidget, SIGNAL(fetch_preview(const Stream&)),
                     this,SIGNAL(fetch_preview(const Stream&)));

    QObject::connect(streamItemWidget, SIGNAL(goToPreview()),this,SIGNAL(goToPreview()));

    QObject::connect(streamItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    QObject::connect(streamItemWidget,SIGNAL(add_favorite(const Stream&)),
                     this,SIGNAL(add_favorite(const Stream&)));
}

void StreamsWidget::clear_streams()
{
    ui->streamListWidget->clear();
}

void StreamsWidget::on_backToGamesButton_clicked()
{
    clear_streams();
    emit back_to_games();
}
