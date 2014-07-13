#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <memory>
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
    Ui::GamesWidget *ui;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;

signals:
    void fetch_games(API::SERVICE service);
    void fetch_streams(QString game, API::SERVICE service);
    void go_to_streams();
    void clear_streams();

    // preview
    void go_to_preview();

public slots:
    void add_game(std::shared_ptr<Game> game);

private slots:
    void search_game(QString game);
    void fetch_streams_by_game(const QModelIndex& index);
    void on_twitchButton_clicked();
};

#endif // GAMESWIDGET_H
