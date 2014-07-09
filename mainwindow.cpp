#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "streamitemwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    livestream(new LiveStream(parent))

{
    ui->setupUi(this);

    // setup the livestream Process
    setup_livestream();

    //setup the games proxy and it's model
    setup_games_model();

    // setup the network manager
    setup_network_manager();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Slot: receives signal from play button
 */
void MainWindow::play(QString url) {

    // clear the output
    ui->liveStreamOutput->clear();

    // if we got no livestream running
    if(livestream->state() == QProcess::NotRunning) {
        ui->adressEdit->setText(url);
        emit play(url, ui->qualityComboBox->currentText());
        return;
    }

    // emit a signal to kill livestreamer
    emit terminate_stream();

    QEventLoop loop;
    QObject::connect(livestream.get(), SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));

    // wait for the process to die
    loop.exec();

    // proceed as usual
    ui->adressEdit->setText(url);
    emit play(url, ui->qualityComboBox->currentText());
}

void MainWindow::livestream_started()
{
    ui->adressEdit->setReadOnly(true);
    ui->playButton->setDisabled(true);
    ui->stopButton->setEnabled(true);
}

void MainWindow::livestream_finished()
{
    ui->adressEdit->setReadOnly(false);
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

    // connect a click event to browsing streams
    QObject::connect(ui->gameListWidget, SIGNAL(activated(QModelIndex)),
                     this, SLOT(fetch_streams_by_game(QModelIndex)));
}

void MainWindow::setup_network_manager()
{
    // connect the add twitch games to the corresponding slot
    QObject::connect(this, SIGNAL(fetch_games(API::SERVICE)),
                     &network, SLOT(fetch_games(API::SERVICE)));

    // connect browsing streams by game
    QObject::connect(this, SIGNAL(fetch_streams(QString,API::SERVICE)),
                     &network, SLOT(fetch_streams_by_game(QString,API::SERVICE)));

    // connect browsing preview
    QObject::connect(this, SIGNAL(fetch_preview(QString,API::SERVICE)),
                     &network, SLOT(fetch_preview(QString,API::SERVICE)));

    // connect the signal to add games
    QObject::connect(&network, SIGNAL(add_game(QString,QString,QString,API::SERVICE)),
                     this, SLOT(add_game(QString,QString,QString,API::SERVICE)));

    // connect the signal to add stream
    QObject::connect(&network, SIGNAL(add_stream(QString,QString,QString,QString,QString,QString,API::SERVICE)),
                     this, SLOT(add_stream(QString,QString,QString,QString,QString,QString,API::SERVICE)));

    // connect signal to show preview
    QObject::connect(&network, SIGNAL(set_preview(QString,QString,QString,QString,QString,QString,QString,API::SERVICE)),
                     ui->PreviewStreamWidget, SLOT(set_preview(QString,QString,QString,QString,QString,QString,QString,API::SERVICE)));

    // connect signal to reset preview
    QObject::connect(&network, SIGNAL(reset_preview()),
                     ui->PreviewStreamWidget, SLOT(reset_preview()));
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

void MainWindow::add_game(QString name, QString viewers, QString nr_of_chans, API::SERVICE service) {
    QStandardItem* item = new QStandardItem(name);
    item->setEditable(false);
    item->setData(name, ROLE_NAME);
    item->setData(viewers, ROLE_VIEWERS);
    item->setData(service, ROLE_SERVICE);
    item->setData(nr_of_chans, ROLE_CHANNEL_NR);
    gamesModel.appendRow(item);
}

void MainWindow::fetch_streams_by_game(const QModelIndex &index)
{
    ui->browseStackedWidget->setCurrentIndex(1);
    ui->streamListWidget->clear();

    QString name = index.data(ROLE_NAME).toString();
    API::SERVICE service = static_cast<API::SERVICE>(index.data(ROLE_SERVICE).toInt());

    emit fetch_streams(name, service);
}

void MainWindow::back_to_games()
{
    ui->streamListWidget->clear();
    ui->browseStackedWidget->setCurrentIndex(0);
}

void MainWindow::add_stream(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service)
{
    QListWidgetItem* item = new QListWidgetItem();
    StreamItemWidget* widget = new StreamItemWidget(streamer,name,status,game,viewers,url,service);
    item->setSizeHint(widget->sizeHint());
    ui->streamListWidget->addItem(item);



    QObject::connect(widget, SIGNAL(preview(QString,API::SERVICE)),this,SLOT(preview(QString,API::SERVICE)));
    QObject::connect(widget, SIGNAL(play(QString)),this,SLOT(play(QString)));
    ui->streamListWidget->setItemWidget(item,widget);
}

void MainWindow::preview(QString streamer, API::SERVICE service)
{
    emit fetch_preview(streamer,service);
}

void MainWindow::on_playButton_clicked()
{
    play(ui->adressEdit->text());
}
