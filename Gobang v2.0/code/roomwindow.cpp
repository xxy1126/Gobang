#include "roomwindow.h"
#include "mainwindow.h"
#include "ui_roomwindow.h"

#include <QString>
#include <QDebug>
RoomWindow::RoomWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomWindow)
{
    ui->setupUi(this);

    ui->groupBox->setStyleSheet("QGroupBox{border:none}");
    ui->groupBox_2->setStyleSheet("QGroupBox{border:none}");
    ui->groupBox_3->setStyleSheet("QGroupBox{border:none}");
//    setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
//                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);
    setWindowTitle("Create Room");

    setWindowIcon(QIcon(":/res/window.jpg"));
}

RoomWindow::~RoomWindow()
{
    delete ui;
}

void RoomWindow::on_pushButton_clicked()
{
    netWorkGameServer = new NetWorkGameServer;
    netWorkGameServer->port = ui->lineEdit->text().toInt();
    //qDebug() << netWorkGameServer->port;
    netWorkGameServer->mainSceneOnline = new MainSceneOnline;
    netWorkGameServer->mainSceneOnline->setGeometry(this->geometry());
    this->close();
    netWorkGameServer->startConnect();
    netWorkGameServer->mainSceneOnline->show();
}



void RoomWindow::on_returnButton_clicked()
{
    MainWindow *nextMainWindow = new MainWindow;
    nextMainWindow->setGeometry(this->geometry());
    this->close();
    nextMainWindow->show();
}

void RoomWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/bg.png");
    painter.drawPixmap(-300,-300,1500,1500,pix);
}
