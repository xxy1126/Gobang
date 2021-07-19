#include "networkgameserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

#include "joinwindow.h"
#include "roomwindow.h"

NetWorkGameServer::NetWorkGameServer(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Server");
}

NetWorkGameServer::~NetWorkGameServer()
{

}

void NetWorkGameServer::startConnect()
{
    m_server = new QTcpServer(this);

    mainSceneOnline->connectLabel->setText("准备连接");
    connect(m_server, &QTcpServer::newConnection, this, [=](){
        mainSceneOnline->connectSuccess = 1;
        mainSceneOnline->connectLabel->setText("连接成功");
        m_tcp = m_server->nextPendingConnection();
        //connected
        connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
            QString recvmsg = m_tcp->readAll();
            int recv = recvmsg.toInt();
            if(recv /10000 == 1)
            {
                mainSceneOnline->giveUpByEnemy();
            }
            else if(recv / 10000 == 2)
            {
                mainSceneOnline->repentanceByEnemy();
            }
            else if(recv / 10000 == 3)
            {
                if(recv%2 == 1) //yes
                {
                    mainSceneOnline->yesRegret();
                }
                else  // no
                {
                    mainSceneOnline->noRegret();
                }

            }
            else
            {
                mainSceneOnline->clickPosCol = recv % 100;
                mainSceneOnline->clickPosRow = recv / 100;
                mainSceneOnline->chessOneByOther();
            }

        });

        connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
           m_tcp->deleteLater();
           mainSceneOnline->connectLabel->setText("连接断开");
           this->close();
           QMessageBox::information(mainSceneOnline, "网络异常", "连接断开，即将返回");
           mainSceneOnline->close();
           RoomWindow *nextRoomWindow = new RoomWindow;
           nextRoomWindow->show();
        });
    });

    connect(this->mainSceneOnline, &MainSceneOnline::netWorkSendMessage,
            this, &NetWorkGameServer::netWorkSendMessage);

    connect(this->mainSceneOnline, &MainSceneOnline::sendGiveUp,
            this, &NetWorkGameServer::sendGiveup);


    connect(this->mainSceneOnline, &MainSceneOnline::sendregret,
            this, &NetWorkGameServer::sendregret);

    connect(this->mainSceneOnline, &MainSceneOnline::sendYesMessage,
            this, &NetWorkGameServer::sendYesMessage);

    connect(this->mainSceneOnline, &MainSceneOnline::sendNoMessage,
            this, &NetWorkGameServer::sendNoMessage);

    m_server->listen(QHostAddress::Any, this->port);
    return ;
}

void NetWorkGameServer::netWorkSendMessage()
{
    int sendNum = mainSceneOnline->clickPosRow*100 + mainSceneOnline->clickPosCol;
    QString sendMsg = QString::number(sendNum, 10);
    //mainSceneOnline->setWindowTitle("sendMsg is " + sendMsg);
    m_tcp->write(sendMsg.toUtf8());
}
void NetWorkGameServer::sendGiveup()
{
    int sendNum = 10000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}
void NetWorkGameServer::sendregret()
{
    int sendNum = 20000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}


void NetWorkGameServer::sendYesMessage()
{
    int sendNum = 30001;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}
void NetWorkGameServer::sendNoMessage()
{
    int sendNum = 30000;
    QString sendMsg = QString::number(sendNum, 10);
    m_tcp->write(sendMsg.toUtf8());
}
