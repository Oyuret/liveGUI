#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "livestream.h"
#include "networkmanager.h"

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

    // setup connections and other stuff
    void setup_livestream();
    void setup_preview();
    void setup_favorites();
    void setup_browsingWidget();

    // settings
    void load_settings();
    void save_settings();

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

    // preview stuff
    void goToPreview();

private slots:
    void on_playButton_clicked();

    // livestream starts/ends
    void livestream_started();
    void livestream_finished();

    // button events
    void on_stopButton_clicked();
};

#endif // MAINWINDOW_H
