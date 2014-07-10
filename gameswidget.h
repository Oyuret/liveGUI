#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "gamesitemdelegate.h"

namespace Ui {
class GamesWidget;
}

class GamesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GamesWidget(QWidget *parent = 0);
    ~GamesWidget();

private:
    Ui::GamesWidget *ui;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;

signals:
    void fetch_games(API::SERVICE);
    void fetch_streams(QString game, API::SERVICE);
    void go_to_streams();
    void clear_streams();

public slots:
    void add_game(QString name, QString viewers, QString nr_of_chans, API::SERVICE service);

private slots:
    void search_game(QString game);
    void fetch_streams_by_game(const QModelIndex& index);
    void on_twitchButton_clicked();
};

#endif // GAMESWIDGET_H