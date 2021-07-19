#ifndef JOINWINDOW_H
#define JOINWINDOW_H

#include <QWidget>
#include "networkgameclient.h"

namespace Ui {
class JoinWindow;
}

class JoinWindow : public QWidget
{
    Q_OBJECT

public:
    explicit JoinWindow(QWidget *parent = nullptr);
    ~JoinWindow();

    int isConnect = 0;

    void goToRoom();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void paintEvent(QPaintEvent *);
private:
    Ui::JoinWindow *ui;
    NetWorkGameClient * netWorkGameClient;
};

#endif // JOINWINDOW_H
