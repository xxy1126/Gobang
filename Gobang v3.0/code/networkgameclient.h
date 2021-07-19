#ifndef NETWORKGAMECLIENT_H
#define NETWORKGAMECLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "mainsceneclient.h"

namespace Ui {
class NetWorkGameClient;
}

class NetWorkGameClient : public QWidget
{
    Q_OBJECT

public:
    explicit NetWorkGameClient(QWidget *parent = nullptr);
    ~NetWorkGameClient();

    QString ip;
    QTcpSocket *m_tcp;
    MainSceneClient *mainSceneClient;

    unsigned short port;
    int connectSuccess = 0;
    void startConnect();
signals:
    void successConnect();

private slots:
//    void on_pushButton_clicked();
    void netWorkSendMessage();
    void sendGiveup();

    void sendregret();

    void sendYesMessage();
    void sendNoMessage();

private:
    Ui::NetWorkGameClient *ui;
};

#endif // NETWORKGAMECLIENT_H
