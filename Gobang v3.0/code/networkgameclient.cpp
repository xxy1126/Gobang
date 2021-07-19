#include "networkgameclient.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <ctime>
#include "roomwindow.h"
#include "joinwindow.h"
#include "mainwindow.h"

NetWorkGameClient::NetWorkGameClient(QWidget *parent) :
    QWidget(parent)
{
    mainSceneClient = new MainSceneClient;
}

NetWorkGameClient::~NetWorkGameClient()
{

}
void NetWorkGameClient::startConnect()
{
    m_tcp = new QTcpSocket(this);

    connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
        QString recvmsg = m_tcp->readAll();
        int recv = recvmsg.toInt();
        if(recv / 10000 == 1)
        {
            mainSceneClient->giveUpByEnemy();
        }
        if(recv / 10000 == 2)
        {
            mainSceneClient->repentanceByEnemy();
        }
        else if(recv / 10000 == 3)
        {
            if(recv%2 == 1) //yes
            {
                mainSceneClient->yesRegret();
            }
            else  // no
            {
                mainSceneClient->noRegret();
            }
        }
        else
        {
            mainSceneClient->clickPosCol = recv % 100;
            mainSceneClient->clickPosRow = recv /100;
            mainSceneClient->chessOneByOther();
        }

    });

    connect(m_tcp, &QTcpSocket::connected, this, [=](){
        //ui->label->setText("连接成功");
        emit successConnect();
    });

    connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
        this->close();
        QMessageBox::information(mainSceneClient, "网络异常", "连接断开，即将返回");
        mainSceneClient->close();
        JoinWindow *nextJoinWindow = new JoinWindow;
        nextJoinWindow->show();
    });
    connect(this->mainSceneClient,&MainSceneClient::netWorkSendMessage,
            this, &NetWorkGameClient::netWorkSendMessage);

    connect(this->mainSceneClient, &MainSceneClient::sendGiveUp,
            this, &NetWorkGameClient::sendGiveup);


    connect(this->mainSceneClient, &MainSceneClient::sendregret,
            this, &NetWorkGameClient::sendregret);

    connect(this->mainSceneClient, &MainSceneClient::sendYesMessage,
            this, &NetWorkGameClient::sendYesMessage);

    connect(this->mainSceneClient, &MainSceneClient::sendNoMessage,
            this, &NetWorkGameClient::sendNoMessage);

    m_tcp->connectToHost(QHostAddress(ip), port);
}

void NetWorkGameClient::netWorkSendMessage()
{
    int sendNum = mainSceneClient->clickPosRow*100 + mainSceneClient->clickPosCol;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}

void NetWorkGameClient::sendGiveup()
{
    int sendNum = 10000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}

void NetWorkGameClient::sendregret()
{
    int sendNum = 20000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}

void NetWorkGameClient::sendYesMessage()
{
    int sendNum = 30001;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}
void NetWorkGameClient::sendNoMessage()
{
    int sendNum = 30000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}
//(recive function !!!!!! in server)
