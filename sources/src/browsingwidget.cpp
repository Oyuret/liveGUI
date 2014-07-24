#include "include/browsingwidget.h"
#include "ui_browsingwidget.h"

void BrowsingWidget::add_game(const Game& game) {
    ui->gamesWidget->add_game(game);
}

void BrowsingWidget::add_stream(const Stream &stream) {
    ui->streamsWidget->add_stream(stream);
}

void BrowsingWidget::goToGamesWidget()
{
    ui->browsingStackedWidget->setCurrentIndex(GAMES_STACK);
}

void BrowsingWidget::goToStreamsWidget()
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
    QObject::connect(ui->gamesWidget,SIGNAL(fetch_games(const Service&)),
                     this, SIGNAL(fetch_games(const Service&)));

    // connect fetch_streams
    QObject::connect(ui->gamesWidget, SIGNAL(fetch_streams(const Game&)),
                     this, SIGNAL(fetch_streams(const Game&)));

    // connect go to streams
    QObject::connect(ui->gamesWidget,SIGNAL(go_to_streams()),this,SLOT(goToStreamsWidget()));

}

void BrowsingWidget::setupStreamsWidget()
{
    // connect add favorite to the fav widget
    QObject::connect(ui->streamsWidget,SIGNAL(add_favorite(const Stream&)),
                     this,SIGNAL(add_favorite(const Stream&)));

    // connect play to this
    QObject::connect(ui->streamsWidget, SIGNAL(play(QString)),
                     this, SIGNAL(play(QString)));

    // connect preview
    QObject::connect(ui->streamsWidget,SIGNAL(fetch_preview(const Stream&)),
                     this,SIGNAL(fetch_preview(const Stream&)));

    // connect go to preview
    QObject::connect(ui->streamsWidget,SIGNAL(goToPreview()), this, SIGNAL(goToPreview()));

    // connect back_to_games
    QObject::connect(ui->streamsWidget,SIGNAL(back_to_games()),this,SLOT(goToGamesWidget()));
}
