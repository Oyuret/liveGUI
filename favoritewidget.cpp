#include "favoritewidget.h"
#include "ui_favoritewidget.h"

FavoriteWidget::FavoriteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteWidget)
{
    ui->setupUi(this);
}

FavoriteWidget::~FavoriteWidget()
{
    delete ui;
}

void FavoriteWidget::add_favorite(QString streamerName, QString name, QString url, API::SERVICE service)
{
    QListWidgetItem* item = new QListWidgetItem();
    FavoriteItemWidget* widget = new FavoriteItemWidget(streamerName, name, url, item, service);
    item->setSizeHint(widget->sizeHint());
    ui->favListWidget->addItem(item);



    QObject::connect(widget, SIGNAL(fetch_preview(QString,API::SERVICE)),this,SIGNAL(fetch_preview(QString,API::SERVICE)));
    QObject::connect(widget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));
    QObject::connect(widget, SIGNAL(remove_favorite(QListWidgetItem*)),this,SLOT(remove_favorite(QListWidgetItem*)));
    ui->favListWidget->setItemWidget(item,widget);
}

void FavoriteWidget::remove_favorite(QListWidgetItem *item)
{
    int row = ui->favListWidget->row(item);
    QListWidgetItem* removed = ui->favListWidget->takeItem(row);
    delete removed;
}

void FavoriteWidget::on_refreshStatusButton_clicked()
{
    // fetch status for each item
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->favListWidget->item(row);
        FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));
        widget->set_button_disabled();
        emit fetch_status(widget->name, widget->service, widget);
    }
}
