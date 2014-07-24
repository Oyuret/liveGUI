#include "../include/gameswidget.h"
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
                     this, SLOT(on_gamesItemActivated(QModelIndex)));

    // searching
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(ui->searchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(searchInGameList(QString)));

}

GamesWidget::~GamesWidget()
{
    delete ui;
}

void GamesWidget::addGame(const Game &game)
{
    QStandardItem* item = new QStandardItem(game.getName());
    item->setEditable(false);
    item->setData(game.getName(), ROLE_NAME);
    item->setData(game.getViewers(), ROLE_VIEWERS);
    item->setData(game.getChannels(), ROLE_CHANNEL_NR);
    item->setData(game.getServiceName(),ROLE_SERVICE_NAME);
    item->setData(game.getServiceLogoResource(), ROLE_SERVICE_LOGO);
    gamesModel.appendRow(item);

}

void GamesWidget::searchInGameList(QString game)
{
    gamesSortProxy.setFilterWildcard(game);
}

void GamesWidget::on_gamesItemActivated(const QModelIndex& index)
{
    emit goToStreamsWidget();

    QString name = index.data(ROLE_NAME).toString();
    QString serviceName = index.data(ROLE_SERVICE_NAME).toString();

    Game game(name,serviceName);

    emit fetchStreamsByGame(game);
}

void GamesWidget::on_twitchButton_clicked()
{
    clearGames();
    Service twitch("twitch");
    emit fetchGamesByService(twitch);
}

void GamesWidget::on_azubuButton_clicked()
{
    clearGames();
}

void GamesWidget::clearGames()
{
    gamesModel.clear();
}
