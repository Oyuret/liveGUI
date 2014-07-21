#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    setup_livestream();
    setup_preview();
    setup_favorites();
    setup_streams();
    setup_games();
    load_settings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Slot: receives signal from any play button
 */
void MainWindow::play(QString url) {

    // clear the output
    ui->liveStreamOutput->clear();

    // if we got no livestream running
    if(livestream.state() == QProcess::NotRunning) {
        ui->adressEdit->setText(url);
        emit play(url, ui->qualityComboBox->currentText());
        return;
    }

    // emit a signal to kill livestreamer
    emit terminate_stream();

    QEventLoop loop;
    QObject::connect(&livestream, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));

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

void MainWindow::setup_livestream() {

    // connect to livestreams play
    QObject::connect(this, SIGNAL(play(QString,QString)),
                     &livestream, SLOT(play(QString,QString)));

    // connect to livestreams end
    QObject::connect(this, SIGNAL(terminate_stream()),
                     &livestream, SLOT(kill()));

    // connect ready to pull stdout
    QObject::connect(&livestream, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(msg_from_livestream()));

    // connect ready to pull err
    QObject::connect(&livestream, SIGNAL(readyReadStandardError()),
                     this, SLOT(err_msg_from_livestream()));

    //connect the started signal to slot
    QObject::connect(&livestream, SIGNAL(started()),
                     this, SLOT(livestream_started()));

    //connect the started signal to slot
    QObject::connect(&livestream, SIGNAL(finished(int,QProcess::ExitStatus)),
                     this, SLOT(livestream_finished()));

}

void MainWindow::setup_preview()
{
    // connect signal to show preview
    QObject::connect(&network, SIGNAL(set_preview(const Stream&)),
                     ui->previewStreamWidget, SLOT(set_preview(const Stream&)));

    // connect signal to reset preview
    QObject::connect(&network, SIGNAL(reset_preview()),
                     ui->previewStreamWidget, SLOT(reset_preview()));

    // connect play to this
    QObject::connect(ui->previewStreamWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

    // connect add favorite to this
    QObject::connect(ui->previewStreamWidget,SIGNAL(add_favorite(const Stream&)),
                     ui->favoriteWidget,SLOT(add_favorite(const Stream&)));
}

void MainWindow::setup_favorites()
{
    // connect go to preview
    QObject::connect(ui->favoriteWidget, SIGNAL(go_to_preview()), this, SLOT(go_to_preview()));

    // connect add favorite refresh to network
    QObject::connect(ui->favoriteWidget,SIGNAL(fetch_status(const Stream&,FavoriteItemWidget*)),
                     &network,SLOT(fetch_stream_status(const Stream&,FavoriteItemWidget*)));

    // connect play
    QObject::connect(ui->favoriteWidget,SIGNAL(play(QString)),
                     this,SLOT(play(QString)));

    // connect preview
    QObject::connect(ui->favoriteWidget,SIGNAL(fetch_preview(const Stream&)),
                     &network,SLOT(fetch_preview(const Stream&)));

    // loading and closing
    QObject::connect(this,SIGNAL(load_favs()),
                     ui->favoriteWidget,SLOT(load_favorites()));

    QObject::connect(this,SIGNAL(save_favs()),ui->favoriteWidget,SLOT(save_favorites()));

}

void MainWindow::setup_streams()
{
    // connect add favorite to the fav widget
    QObject::connect(ui->streamsWidget,SIGNAL(add_favorite(const Stream&)),
                     ui->favoriteWidget,SLOT(add_favorite(const Stream&)));

    // connect play to this
    QObject::connect(ui->streamsWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

    // connect preview
    QObject::connect(ui->streamsWidget,SIGNAL(fetch_preview(const Stream&)),
                     &network,SLOT(fetch_preview(const Stream&)));

    // connect go to preview
    QObject::connect(ui->streamsWidget,SIGNAL(go_to_preview()), this, SLOT(go_to_preview()));

    // connect back_to_games
    QObject::connect(ui->streamsWidget,SIGNAL(back_to_games()),this,SLOT(back_to_games()));

    // connect the signal to add stream
    QObject::connect(&network, SIGNAL(add_stream(const Stream&)),
                     ui->streamsWidget, SLOT(add_stream(const Stream&)));
}

void MainWindow::setup_games()
{
    // connect fetch_games
    QObject::connect(ui->gamesWidget,SIGNAL(fetch_games(const Service&)),
                     &network, SLOT(fetch_games(const Service&)));

    // connect fetch_streams
    QObject::connect(ui->gamesWidget, SIGNAL(fetch_streams(const Game&)),
                     &network, SLOT(fetch_streams_by_game(const Game&)));

    // connect add_games
    QObject::connect(&network, SIGNAL(add_game(const Game&)),
                     ui->gamesWidget, SLOT(add_game(const Game&)));

    // connect go to streams
    QObject::connect(ui->gamesWidget,SIGNAL(go_to_streams()),this,SLOT(go_to_streams()));

    // connect clear streams
    QObject::connect(ui->gamesWidget,SIGNAL(clear_streams()),ui->streamsWidget,SLOT(clear_streams()));
}

void MainWindow::load_settings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString quality = settings.value("quality", "best").toString();
    ui->qualityComboBox->setCurrentIndex(ui->qualityComboBox->findText(quality));

    emit load_favs();
}

void MainWindow::save_settings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    settings.setValue("quality", ui->qualityComboBox->currentText());

    emit save_favs();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    save_settings();
}

/**
 * @brief Slot: announces there is data to pull from livestream
 */
void MainWindow::msg_from_livestream() {
    QByteArray msgs = livestream.readAllStandardOutput();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit update_output(msg);
    }
}

/**
 * @brief Slot: announces there is error data to pull from livestream
 */
void MainWindow::err_msg_from_livestream() {
    QByteArray msgs = livestream.readAllStandardError();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit update_output(msg);
    }
}


void MainWindow::back_to_games()
{
    ui->browseStackedWidget->setCurrentIndex(GAMES_STACK);
}

void MainWindow::go_to_streams()
{
    ui->browseStackedWidget->setCurrentIndex(STREAMS_STACK);
}

void MainWindow::go_to_preview()
{
    ui->tabWidget->setCurrentIndex(PREVIEW_TAB);
}

void MainWindow::on_playButton_clicked()
{
    play(ui->adressEdit->text());
}

void MainWindow::on_stopButton_clicked()
{
    emit terminate_stream();
}
