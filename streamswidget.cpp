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

void StreamsWidget::add_stream(std::shared_ptr<Stream> stream)
{
    QListWidgetItem* streamItem = new QListWidgetItem();
    StreamItemWidget* streamItemWidget = new StreamItemWidget(stream->getDisplayName(),stream->getChannelName(),
                                                              stream->getStatus(),stream->getGame(),stream->getViewers(),
                                                              stream->getUrl(),stream->getService());

    streamItem->setSizeHint(streamItemWidget->sizeHint());
    ui->streamListWidget->addItem(streamItem);



    // pass on preview
    QObject::connect(streamItemWidget, SIGNAL(fetch_preview(QString,API::SERVICE)),
                     this,SIGNAL(fetch_preview(QString,API::SERVICE)));
    QObject::connect(streamItemWidget, SIGNAL(go_to_preview()),this,SIGNAL(go_to_preview()));

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
