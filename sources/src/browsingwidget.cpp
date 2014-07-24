#include "include/browsingwidget.h"
#include "ui_browsingwidget.h"

void BrowsingWidget::on_addGame(const Game& game) {
    ui->gamesWidget->addGame(game);
}

void BrowsingWidget::on_addStream(const Stream &stream) {
    ui->streamsWidget->addStream(stream);
}

void BrowsingWidget::on_goToGamesWidget()
{
    ui->browsingStackedWidget->setCurrentIndex(GAMES_STACK);
}

void BrowsingWidget::on_goToStreamsWidget()
{
    ui->browsingStackedWidget->setCurrentIndex(STREAMS_STACK);
}

BrowsingWidget::BrowsingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowsingWidget)
{
    ui->setupUi(this);
    setupGamesWidget();
    setupStreamsWidget();
}

BrowsingWidget::~BrowsingWidget()
{
    delete ui;
}

void BrowsingWidget::setupGamesWidget()
{
    // connect fetch_games
    QObject::connect(ui->gamesWidget,SIGNAL(fetchGamesByService(const Service&)),
                     this, SIGNAL(fetchGamesByService(const Service&)));

    // connect fetch_streams
    QObject::connect(ui->gamesWidget, SIGNAL(fetchStreamsByGame(const Game&)),
                     this, SIGNAL(fetchStreamsByGame(const Game&)));

    // connect go to streams
    QObject::connect(ui->gamesWidget,SIGNAL(goToStreamsWidget()),this,SLOT(on_goToStreamsWidget()));

}

void BrowsingWidget::setupStreamsWidget()
{
    // connect add favorite to the fav widget
    QObject::connect(ui->streamsWidget,SIGNAL(addFavorite(const Stream&)),
                     this,SIGNAL(addFavorite(const Stream&)));

    // connect play to this
    QObject::connect(ui->streamsWidget, SIGNAL(play(QString)),
                     this, SIGNAL(play(QString)));

    // connect preview
    QObject::connect(ui->streamsWidget,SIGNAL(fetchStreamPreview(const Stream&)),
                     this,SIGNAL(fetchStreamPreview(const Stream&)));

    // connect go to preview
    QObject::connect(ui->streamsWidget,SIGNAL(goToPreview()), this, SIGNAL(goToPreview()));

    // connect back_to_games
    QObject::connect(ui->streamsWidget,SIGNAL(goToGamesWidget()),this,SLOT(on_goToGamesWidget()));
}
