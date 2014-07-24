#ifndef FAVORITEITEMWIDGET_H
#define FAVORITEITEMWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "stream.h"

namespace Ui {
class FavoriteItemWidget;
}

class FavoriteItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteItemWidget(const Stream& stream, QListWidgetItem* item);
    ~FavoriteItemWidget();

    void setOnline();
    void setOffline();
    void setChecking();

    Stream getStream() const;

signals:
    void play(QString url);
    void fetchStreamPreview(const Stream& stream);
    void removeFavorite(QListWidgetItem* item);
    void goToPreview();

private:
    Ui::FavoriteItemWidget *ui;
    QListWidgetItem* item;
    Stream stream;

public slots:


private slots:
    void on_playStreamButton_clicked();
    void on_previewStreamButton_clicked();
    void on_removeFavoriteButton_clicked();
};

#endif // FAVORITEITEMWIDGET_H
