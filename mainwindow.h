#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <memory>
#include <QModelIndex>
#include "livestream.h"
#include "gamesitemdelegate.h"
#include "networkmanager.h"
#include "streamitemwidget.h"
#include "favoriteitemwidget.h"

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

    // The process hosting livestreamer
    LiveStream livestream;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;

    // The network manager in charge of making requests
    NetworkManager network;

    // setup connections and other stuff
    void setup_livestream();
    void setup_games_model();
    void setup_network_manager();
    void setup_preview();

signals:
    void play(QString adress, QString quality);
    void terminate_stream();
    void update_output(QString msg);

    // Games browsing
    void fetch_games(API::SERVICE service);
    void fetch_streams(QString name, API::SERVICE service);
    void fetch_preview(QString name, API::SERVICE service);

public slots:

    // Livestream stuff
    void play(QString url);
    void end_stream();
    void msg_from_livestream();
    void err_msg_from_livestream();

    // Games browsing stuff
    void games_search(QString msg);
    void fetch_twitch_games();
    void fetch_azubu_games();
    void add_game(QString name, QString viewers, QString nr_of_chans,API::SERVICE service);

    //Stream browsing stuff
    void fetch_streams_by_game(const QModelIndex & index);
    void back_to_games();
    void add_stream(QString streamer, QString name, QString status, QString game, QString viewers, QString url, API::SERVICE service);

    // Preview stuff
    void preview(QString streamer, API::SERVICE service);

    // favorites stuff
    void add_favorite(QString streamerName, QString name, QString url, API::SERVICE service);
    void remove_favorite(QListWidgetItem*item);

private slots:
    void on_playButton_clicked();

    // livestream starts/ends
    void livestream_started();
    void livestream_finished();


    void on_refreshFavoritesButton_clicked();
};

#endif // MAINWINDOW_H
