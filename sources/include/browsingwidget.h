#ifndef BROWSINGWIDGET_H
#define BROWSINGWIDGET_H

#include <QWidget>
#include "game.h"
#include "stream.h"
#include "service.h"

namespace Ui {
class BrowsingWidget;
}

class BrowsingWidget : public QWidget
{
    Q_OBJECT

    enum Stacks {
        GAMES_STACK,
        STREAMS_STACK
    };

signals:
    void fetch_preview(const Stream& stream);
    void fetch_streams(const Game& game);
    void fetch_games(const Service& service);
    void add_favorite(const Stream& stream);
    void play(QString url);
    void goToPreview();

public slots:
    void add_game(const Game& game);
    void add_stream(const Stream& stream);

private slots:
    void goToGamesWidget();
    void goToStreamsWidget();

public:
    explicit BrowsingWidget(QWidget *parent = 0);
    ~BrowsingWidget();

private:
    void setupGamesWidget();
    void setupStreamsWidget();
    Ui::BrowsingWidget *ui;
};

#endif // BROWSINGWIDGET_H
