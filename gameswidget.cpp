#include "gameswidget.h"
#include "ui_gameswidget.h"

GamesWidget::GamesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamesWidget)
{
    ui->setupUi(this);

    gamesSortProxy.setSourceModel(&gamesModel);
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->gamesListView->setModel(&gamesSortProxy);
    ui->gamesListView->setItemDelegate(&gamesDelegate);

    // connect an activated event to browsing streams
    QObject::connect(ui->gamesListView, SIGNAL(activated(QModelIndex)),
                     this, SLOT(fetch_streams_by_game(QModelIndex)));

    // searching
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(ui->searchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(search_game(QString)));

}

GamesWidget::~GamesWidget()
{
    delete ui;
}

void GamesWidget::add_game(QString name, QString viewers, QString nr_of_chans, API::SERVICE service)
{
    QStandardItem* item = new QStandardItem(name);
    item->setEditable(false);
    item->setData(name, ROLE_NAME);
    item->setData(viewers, ROLE_VIEWERS);
    item->setData(service, ROLE_SERVICE);
    item->setData(nr_of_chans, ROLE_CHANNEL_NR);
    gamesModel.appendRow(item);
}

void GamesWidget::search_game(QString game)
{
    gamesSortProxy.setFilterWildcard(game);
}

void GamesWidget::fetch_streams_by_game(const QModelIndex& index)
{
    emit go_to_streams();
    emit clear_streams();

    QString name = index.data(ROLE_NAME).toString();
    API::SERVICE service = static_cast<API::SERVICE>(index.data(ROLE_SERVICE).toInt());

    emit fetch_streams(name, service);
}

void GamesWidget::on_twitchButton_clicked()
{
    emit fetch_games(API::TWITCH);
}
