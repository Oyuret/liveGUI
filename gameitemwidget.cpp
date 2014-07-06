#include "gameitemwidget.h"
#include "ui_gameitemwidget.h"

GameItemWidget::GameItemWidget(QString game, QString viewers) :
    QWidget(),
    ui(new Ui::GameItemWidget)
{
    ui->setupUi(this);
    ui->gameLine->setText(game);
    ui->viewersLine->setText(viewers);
}

GameItemWidget::~GameItemWidget()
{
    delete ui;
}
