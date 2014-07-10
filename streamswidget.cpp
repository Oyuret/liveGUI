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

void StreamsWidget::add_stream(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service)
{
    QListWidgetItem* item = new QListWidgetItem();
    StreamItemWidget* widget = new StreamItemWidget(streamer,name,status,game,viewers,url,service);
    item->setSizeHint(widget->sizeHint());
    ui->streamListWidget->addItem(item);



    // pass on preview
    QObject::connect(widget, SIGNAL(fetch_preview(QString,API::SERVICE)),this,SIGNAL(fetch_preview(QString,API::SERVICE)));

    // pass on play
    QObject::connect(widget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    // pass on add_favorite
    QObject::connect(widget,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)),
                     this,SIGNAL(add_favorite(QString,QString,QString,API::SERVICE)));

    ui->streamListWidget->setItemWidget(item,widget);
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
