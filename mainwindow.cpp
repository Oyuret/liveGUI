#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameitemwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    livestream(new LiveStream(parent)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup the livestream Process
    setup_livestream();

    //setup the games proxy and it's model
    setup_games_model();

    // setup the network manager
    setup_network_manager();

    // Debug studd
    populate_games();

}

MainWindow::~MainWindow()
{
    delete ui;
    //delete livestream;
}

/**
 * @brief Slot: receives signal from play button
 */
void MainWindow::play() {
    emit play("http://www.twitch.tv/mushisgosu", ui->qualityComboBox->currentText());
}

void MainWindow::livestream_started()
{
    ui->playButton->setDisabled(true);
    ui->stopButton->setEnabled(true);
}

void MainWindow::livestream_finished()
{
    ui->playButton->setEnabled(true);
    ui->stopButton->setDisabled(true);
}

/**
 * @brief Slot: recieves signal from stop button
 */
void MainWindow::end_stream() {
    emit terminate_stream();
}

void MainWindow::setup_livestream() {

    // connect to livestreams play
    QObject::connect(this, SIGNAL(play(QString,QString)),
                     livestream.get(), SLOT(play(QString,QString)));

    // connect to livestreams end
    QObject::connect(this, SIGNAL(terminate_stream()),
                     livestream.get(), SLOT(kill()));

    // connect ready to pull stdout
    QObject::connect(livestream.get(), SIGNAL(readyReadStandardOutput()),
                     this, SLOT(msg_from_livestream()));

    // connect ready to pull err
    QObject::connect(livestream.get(), SIGNAL(readyReadStandardError()),
                     this, SLOT(err_msg_from_livestream()));

    //connect the started signal to slot
    QObject::connect(livestream.get(), SIGNAL(started()),
                     this, SLOT(livestream_started()));

    //connect the started signal to slot
    QObject::connect(livestream.get(), SIGNAL(finished(int,QProcess::ExitStatus)),
                     this, SLOT(livestream_finished()));

}

void MainWindow::setup_games_model() {
    gamesSortProxy.setSourceModel(&gamesModel);
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->gameListWidget->setModel(&gamesSortProxy);
    ui->gameListWidget->setItemDelegate(&gamesDelegate);
}

void MainWindow::setup_network_manager()
{
    //connect the add twitch games to the corresponding slot
    QObject::connect(this, SIGNAL(fetch_games(API::SERVICE)),
                     &network, SLOT(fetch_games(API::SERVICE)));

    // connect the signal to add games
    QObject::connect(&network, SIGNAL(add_game(QString,QString,API::SERVICE)),
                     this, SLOT(add_game(QString,QString,API::SERVICE)));
}

void MainWindow::populate_games() {

    QStandardItem* test = new QStandardItem("LoL");
    test->setEditable(false);
    test->setData("lol", ROLE_NAME);
    test->setData("300", ROLE_VIEWERS);
    gamesModel.appendRow(test);

    test = new QStandardItem("WoW");
    test->setEditable(false);
    test->setData("WoW", ROLE_NAME);
    test->setData("400", ROLE_VIEWERS);
    gamesModel.appendRow(test);

    test = new QStandardItem("Dota");
    test->setEditable(false);
    test->setData("Dota", ROLE_NAME);
    test->setData("500", ROLE_VIEWERS);
    gamesModel.appendRow(test);

    test = new QStandardItem("CS");
    test->setEditable(false);
    test->setData("CS", ROLE_NAME);
    test->setData("500", ROLE_VIEWERS);
    gamesModel.appendRow(test);


    /*gamesModel.appendRow(new QStandardItem("WoW"));
    gamesModel.appendRow(new QStandardItem("Dota"));
    gamesModel.appendRow(new QStandardItem("1"));
    gamesModel.appendRow(new QStandardItem("123"));
    */
}

/**
 * @brief Slot: announces there is data to pull from livestream
 */
void MainWindow::msg_from_livestream() {
    QByteArray msgs = livestream->readAllStandardOutput();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit update_output(msg);
    }
}

/**
 * @brief Slot: announces there is error data to pull from livestream
 */
void MainWindow::err_msg_from_livestream() {
    QByteArray msgs = livestream->readAllStandardError();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit update_output(msg);
    }
}

/**
 * @brief Signals we should filter the result list
 * @param msg The string to filter
 */
void MainWindow::games_search(QString msg) {
    gamesSortProxy.setFilterWildcard(msg);
}

/**
 * @brief Fetch all twitch games
 */
void MainWindow::fetch_twitch_games() {
    gamesModel.clear();
    emit fetch_games(API::TWITCH);
}

/**
 * @brief Fetches all games by azubu
 */
void MainWindow::fetch_azubu_games() {
    gamesModel.clear();
    qDebug() << "Fetching azubu games";
}

void MainWindow::add_game(QString name, QString viewers, API::SERVICE service) {
    QStandardItem* item = new QStandardItem(name);
    item->setEditable(false);
    item->setData(name, ROLE_NAME);
    item->setData(viewers, ROLE_VIEWERS);
    item->setData(service, ROLE_SERVICE);
    gamesModel.appendRow(item);
}
