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
    void fetchStreamPreview(const Stream& stream);
    void fetchStreamsByGame(const Game& game);
    void fetchGamesByService(const Service& service);
    void addFavorite(const Stream& stream);
    void play(QString url);
    void goToPreview();

public slots:
    void on_addGame(const Game& game);
    void on_addStream(const Stream& stream);

private slots:
    void on_goToGamesWidget();
    void on_goToStreamsWidget();

public:
    explicit BrowsingWidget(QWidget *parent = 0);
    ~BrowsingWidget();

private:
    void setupGamesWidget();
    void setupStreamsWidget();
    Ui::BrowsingWidget *ui;
};

#endif // BROWSINGWIDGET_H
