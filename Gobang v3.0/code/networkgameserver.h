#ifndef NETWORKGAMESERVER_H
#define NETWORKGAMESERVER_H

#include <QWidget>
#include <QTcpServer>
#include "mainsceneonline.h"


namespace Ui {
class NetWorkGameServer;
}

class NetWorkGameServer : public QWidget
{
    Q_OBJECT

public:
    explicit NetWorkGameServer(QWidget *parent = nullptr);
    ~NetWorkGameServer();

    MainSceneOnline *mainSceneOnline;
    QTcpServer *m_server;
    QTcpSocket *m_tcp;
    unsigned short port;
    int sendX, sendY, recvX, recvY;
    void startConnect();

private slots:
//    void on_pushButton_clicked();
    void netWorkSendMessage();

    void sendGiveup();

    void sendregret();

    void sendYesMessage();
    void sendNoMessage();

private:
    Ui::NetWorkGameServer *ui;
};

#endif // NETWORKGAMESERVER_H
