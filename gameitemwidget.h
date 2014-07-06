#ifndef GAMEITEMWIDGET_H
#define GAMEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class GameItemWidget;
}

class GameItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameItemWidget(QWidget *parent = 0);
    GameItemWidget(QString game, QString viewers);
    ~GameItemWidget();

private:
    Ui::GameItemWidget *ui;
};

#endif // GAMEITEMWIDGET_H
