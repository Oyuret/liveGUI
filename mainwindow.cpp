#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameitemwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    livestream(new LiveStream(parent)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup_livestream();
    setup_game_list();

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

void MainWindow::setup_game_list() {
    GameItemWidget* test = new GameItemWidget("testtestestsetsetsetsetset", "400");
    GameItemWidget* test2 = new GameItemWidget("test2", "4000");
    QListWidgetItem* t1 = new QListWidgetItem();
    QListWidgetItem* t2 = new QListWidgetItem();
    t1->setSizeHint(test->sizeHint());
    t2->setSizeHint(test2->sizeHint());
    ui->gameListWidget->addItem(t1);
    ui->gameListWidget->addItem(t2);
    ui->gameListWidget->setItemWidget(t1,test);
    ui->gameListWidget->setItemWidget(t2,test2);
}
