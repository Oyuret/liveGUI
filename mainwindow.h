#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <memory>
#include "livestream.h"
#include "gamesitemdelegate.h"
#include "networkmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<LiveStream> livestream;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;
    NetworkManager network;

    void setup_livestream();
    void setup_games_model();
    void setup_network_manager();

    //DEBUG STUFF
    void populate_games();

signals:
    void play(QString adress, QString quality);
    void terminate_stream();
    void update_output(QString msg);

    // Games browsing
    void fetch_games(API::SERVICE service);

public slots:

    // Livestream stuff
    void play();
    void livestream_started();
    void livestream_finished();
    void end_stream();
    void msg_from_livestream();
    void err_msg_from_livestream();

    // Games browsing stuff
    void games_search(QString msg);
    void fetch_twitch_games();
    void fetch_azubu_games();
    void add_game(QString name, QString viewers, API::SERVICE service);

private slots:
};

#endif // MAINWINDOW_H
