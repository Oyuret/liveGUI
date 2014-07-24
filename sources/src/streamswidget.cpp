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

void StreamsWidget::addStream(const Stream &stream)
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
    QObject::connect(streamItemWidget, SIGNAL(fetchStreamPreview(const Stream&)),
                     this,SIGNAL(fetchStreamPreview(const Stream&)));

    QObject::connect(streamItemWidget, SIGNAL(goToPreview()),this,SIGNAL(goToPreview()));

    QObject::connect(streamItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    QObject::connect(streamItemWidget,SIGNAL(addFavorite(const Stream&)),
                     this,SIGNAL(addFavorite(const Stream&)));
}

void StreamsWidget::clear_streams()
{
    ui->streamListWidget->clear();
}

void StreamsWidget::on_backToGamesButton_clicked()
{
    clear_streams();
    emit goToGamesWidget();
}
