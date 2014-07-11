#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <memory>
#include <QModelIndex>
#include "livestream.h"
#include "networkmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum TABS {
        GAMES_TAB,
        STREAMS_TAB
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

    // setup connections and other stuff
    void setup_livestream();
    void setup_preview();
    void setup_favorites();
    void setup_streams();
    void setup_games();

    // settings
    void load_settings();
    void save_settings();

    // debug stuff
    void populate_favs();

    // closing
    void closeEvent(QCloseEvent *event);

signals:
    void play(QString adress, QString quality);
    void terminate_stream();
    void update_output(QString msg);
    void load_favs();
    void save_favs();


public slots:

    // Livestream stuff
    void play(QString url);
    void msg_from_livestream();
    void err_msg_from_livestream();

    //Stream browsing stuff
    void back_to_games();
    void go_to_streams();

private slots:
    void on_playButton_clicked();

    // livestream starts/ends
    void livestream_started();
    void livestream_finished();

    // button events
    void on_stopButton_clicked();
};

#endif // MAINWINDOW_H
