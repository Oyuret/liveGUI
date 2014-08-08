#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    setupLivestream();
    setupPreview();
    setupFavorites();
    setupBrowsingwidget();
    loadSettings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::play(QString url) {

    if(remotePlayIsChecked()) {
        remote.play(url,getRemotePlayerAddress());
        return;
    }

    // clear the output
    ui->liveStreamOutput->clear();

    // if we got no livestream running
    if(livestream.state() == QProcess::NotRunning) {
        ui->adressEdit->setText(url);
        emit play(url, ui->qualityComboBox->currentText());
        return;
    }

    // emit a signal to kill livestreamer
    emit terminateStream();

    QEventLoop loop;
    QObject::connect(&livestream, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));

    // wait for the process to die
    loop.exec();

    // proceed as usual
    ui->adressEdit->setText(url);
    emit play(url, ui->qualityComboBox->currentText());
}

void MainWindow::on_livestreamStarted()
{
    ui->adressEdit->setReadOnly(true);
    ui->playButton->setDisabled(true);
    ui->stopButton->setEnabled(true);
}

void MainWindow::on_livestreamFinished()
{
    ui->adressEdit->setReadOnly(false);
    ui->playButton->setEnabled(true);
    ui->stopButton->setDisabled(true);
}

void MainWindow::setupLivestream() {

    // connect to livestreams play
    QObject::connect(this, SIGNAL(play(QString,QString)),
                     &livestream, SLOT(play(QString,QString)));

    // connect to livestreams end
    QObject::connect(this, SIGNAL(terminateStream()),
                     &livestream, SLOT(kill()));

    // connect ready to pull stdout
    QObject::connect(&livestream, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(on_msgFromLivestream()));

    // connect ready to pull err
    QObject::connect(&livestream, SIGNAL(readyReadStandardError()),
                     this, SLOT(on_errorMsgFromLivestream()));

    //connect the started signal to slot
    QObject::connect(&livestream, SIGNAL(started()),
                     this, SLOT(on_livestreamStarted()));

    //connect the started signal to slot
    QObject::connect(&livestream, SIGNAL(finished(int,QProcess::ExitStatus)),
                     this, SLOT(on_livestreamFinished()));

}

void MainWindow::setupPreview()
{
    // connect signal to show preview
    QObject::connect(&network, SIGNAL(setPreview(const Stream&)),
                     ui->previewStreamWidget, SLOT(on_setPreview(const Stream&)));

    // connect signal to reset preview
    QObject::connect(&network, SIGNAL(resetPreview()),
                     ui->previewStreamWidget, SLOT(on_resetPreview()));

    // connect play to this
    QObject::connect(ui->previewStreamWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

    // connect add favorite to this
    QObject::connect(ui->previewStreamWidget,SIGNAL(addFavorite(const Stream&)),
                     ui->favoriteWidget,SLOT(on_addFavorite(const Stream&)));
}

void MainWindow::setupFavorites()
{
    // connect go to preview
    QObject::connect(ui->favoriteWidget, SIGNAL(goToPreview()), this, SLOT(on_goToPreview()));

    // connect add favorite refresh to network
    QObject::connect(ui->favoriteWidget,SIGNAL(fetchStreamStatus(const Stream&)),
                     &network,SLOT(on_fetchStreamStatus(const Stream&)));

    // connect play
    QObject::connect(ui->favoriteWidget,SIGNAL(play(QString)),
                     this,SLOT(play(QString)));

    // connect preview
    QObject::connect(ui->favoriteWidget,SIGNAL(fetchStreamPreview(const Stream&)),
                     &network,SLOT(on_fetchStreamPreview(const Stream&)));

    // loading and closing
    QObject::connect(this,SIGNAL(load_favs()),
                     ui->favoriteWidget,SLOT(on_loadFavorites()));

    QObject::connect(this,SIGNAL(save_favs()),
                     ui->favoriteWidget,SLOT(on_saveFavorites()));

    // stream status
    QObject::connect(&network,SIGNAL(streamOnline(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamOnline(const Stream&)));
    QObject::connect(&network,SIGNAL(streamOffline(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamOffline(const Stream&)));
    QObject::connect(&network,SIGNAL(streamUncertain(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamUncertain(const Stream&)));

}

void MainWindow::setupBrowsingwidget()
{
    QObject::connect(ui->browsingWidget,SIGNAL(fetchGamesByService(const Service&)),
                     &network, SLOT(on_fetchGamesByService(const Service&)));
    QObject::connect(&network, SIGNAL(addGame(const Game&)),
                     ui->browsingWidget, SLOT(on_addGame(const Game&)));


    QObject::connect(ui->browsingWidget, SIGNAL(fetchStreamsByGame(const Game&)),
                     &network, SLOT(on_fetchStreamsByGame(const Game&)));
    QObject::connect(&network, SIGNAL(addStream(const Stream&)),
                     ui->browsingWidget, SLOT(on_addStream(const Stream&)));

    QObject::connect(ui->browsingWidget,SIGNAL(fetchStreamPreview(const Stream&)),
                     &network,SLOT(on_fetchStreamPreview(const Stream&)));
    QObject::connect(ui->browsingWidget,SIGNAL(goToPreview()),
                     this, SLOT(on_goToPreview()));

    QObject::connect(ui->browsingWidget,SIGNAL(addFavorite(const Stream&)),
                     ui->favoriteWidget,SLOT(on_addFavorite(const Stream&)));

    QObject::connect(ui->browsingWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

}

void MainWindow::loadSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString quality = settings.value("quality", "best").toString();
    setCurrentQuality(quality);

    bool remotePlayingEnabled = settings.value("remotePlayingEnabled", false).toBool();
    setRemotePlaying(remotePlayingEnabled);

    QString remotePlayerAddress = settings.value("remotePlayerAddress","localhost").toString();
    setRemotePlayerAddress(remotePlayerAddress);

    emit load_favs();
}

void MainWindow::setCurrentQuality(QString quality)
{
    ui->qualityComboBox->setCurrentIndex(ui->qualityComboBox->findText(quality));
}

void MainWindow::saveSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    QString currentQuality = getCurrentQuality();
    settings.setValue("quality", currentQuality);

    bool remotePlayingEnabled = remotePlayIsChecked();
    settings.setValue("remotePlayingEnabled", remotePlayingEnabled);

    QString remotePlayerAddress = getRemotePlayerAddress();
    settings.setValue("remotePlayerAddress",remotePlayerAddress);

    emit save_favs();
}

QString MainWindow::getCurrentQuality() const
{
    return ui->qualityComboBox->currentText();
}

void MainWindow::setRemotePlaying(bool checked)
{
    ui->remotePlayButton->setChecked(checked);
}

bool MainWindow::remotePlayIsChecked() const
{
    return ui->remotePlayButton->isChecked();
}

void MainWindow::setRemotePlayerAddress(QString remoteAddress)
{
    ui->remotePlayerAdressField->setText(remoteAddress);
}

QString MainWindow::getRemotePlayerAddress() const
{
    return ui->remotePlayerAdressField->text();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    saveSettings();
}

void MainWindow::on_msgFromLivestream() {
    QByteArray msgs = livestream.readAllStandardOutput();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit updateLivestreamOutput(msg);
    }
}

void MainWindow::on_errorMsgFromLivestream() {
    QByteArray msgs = livestream.readAllStandardError();
    QStringList strLines = QString(msgs).split("\n");

    for(auto& msg : strLines) {
        emit updateLivestreamOutput(msg);
    }
}

void MainWindow::on_goToPreview()
{
    ui->tabWidget->setCurrentIndex(PREVIEW_TAB);
}

void MainWindow::on_playButton_clicked()
{
    play(ui->adressEdit->text());
}

void MainWindow::on_stopButton_clicked()
{
    if(remotePlayIsChecked()) {
        remote.stop(getRemotePlayerAddress());
        return;
    }
    emit terminateStream();
}
