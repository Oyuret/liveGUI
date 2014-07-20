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
    QObject::connect(streamItemWidget, SIGNAL(fetch_preview(QString,API::SERVICE)),
                     this,SIGNAL(fetch_preview(QString,API::SERVICE)));

    QObject::connect(streamItemWidget, SIGNAL(go_to_preview()),this,SIGNAL(go_to_preview()));

    QObject::connect(streamItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    QObject::connect(streamItemWidget,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)),
                     this,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)));
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
