#include "streamswidget.h"
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

void StreamsWidget::add_stream(QString displayName, QString channelName, QString status, QString game,
                               QString viewers, QString url, API::SERVICE service)
{
    QListWidgetItem* streamItem = new QListWidgetItem();
    StreamItemWidget* streamItemWidget = new StreamItemWidget(displayName,channelName,status,game,viewers,url,service);
    streamItem->setSizeHint(streamItemWidget->sizeHint());
    ui->streamListWidget->addItem(streamItem);



    // pass on preview
    QObject::connect(streamItemWidget, SIGNAL(fetch_preview(QString,API::SERVICE)),this,SIGNAL(fetch_preview(QString,API::SERVICE)));

    // pass on play
    QObject::connect(streamItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    // pass on add_favorite
    QObject::connect(streamItemWidget,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)),
                     this,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)));

    ui->streamListWidget->setItemWidget(streamItem,streamItemWidget);
}

void StreamsWidget::clear_streams()
{
    ui->streamListWidget->clear();
}

void StreamsWidget::on_backToGamesButton_clicked()
{
    ui->streamListWidget->clear();
    emit back_to_games();
}
