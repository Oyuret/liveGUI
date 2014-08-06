#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "livestream.h"
#include "networkmanager.h"
#include "remoteplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum TABS {
        BROWSE_TAB,
        FAVORITE_TAB,
        OUTPUT_TAB,
        PREVIEW_TAB
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // The process hosting livestreamer
    LiveStream livestream;

    // The network manager in charge of making requests
    NetworkManager network;

    // The remote player
    RemotePlayer remote;


    // setup connections and other stuff
    void setupLivestream();
    void setupPreview();
    void setupFavorites();
    void setupBrowsingwidget();

    // settings
    void loadSettings();
    void saveSettings();
    void setCurrentQuality(QString quality);
    QString getCurrentQuality() const;

    // closing
    void closeEvent(QCloseEvent *event);

signals:
    void play(QString adress, QString quality);
    void terminateStream();
    void updateLivestreamOutput(QString msg);
    void load_favs();
    void save_favs();


public slots:

    // Livestream stuff
    void play(QString url);
    void on_msgFromLivestream();
    void on_errorMsgFromLivestream();

    // preview stuff
    void on_goToPreview();

private slots:
    void on_playButton_clicked();

    // livestream starts/ends
    void on_livestreamStarted();
    void on_livestreamFinished();

    // button events
    void on_stopButton_clicked();
};

#endif // MAINWINDOW_H
