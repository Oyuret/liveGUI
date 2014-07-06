#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "livestream.h"

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

    void setup_livestream();

    //DEBUG STUFF
    void setup_game_list();

signals:
    void play(QString adress, QString quality);
    void terminate_stream();
    void update_output(QString msg);

public slots:
    void play();
    void livestream_started();
    void livestream_finished();
    void end_stream();
    void msg_from_livestream();
    void err_msg_from_livestream();
private slots:
};

#endif // MAINWINDOW_H
