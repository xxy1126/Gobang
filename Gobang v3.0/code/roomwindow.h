#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QWidget>
#include "networkgameserver.h"
namespace Ui {
class RoomWindow;
}

class RoomWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RoomWindow(QWidget *parent = nullptr);
    ~RoomWindow();

private slots:
    void on_pushButton_clicked();

    void on_returnButton_clicked();
    void paintEvent(QPaintEvent *);
private:
    Ui::RoomWindow *ui;
    NetWorkGameServer *netWorkGameServer;
};

#endif // ROOMWINDOW_H
