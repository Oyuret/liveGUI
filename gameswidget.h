#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "gamesitemdelegate.h"
#include "game.h"

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
    void clearGames();

    Ui::GamesWidget *ui;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;

signals:
    void fetch_games(const Service& service);
    void fetch_streams(const Game& game);
    void go_to_streams();
    void clear_streams();

public slots:
    void add_game(const Game& game);

private slots:
    void search_game(QString game);
    void fetch_streams_by_game(const QModelIndex& index);
    void on_twitchButton_clicked();
    void on_azubuButton_clicked();
};

#endif // GAMESWIDGET_H
