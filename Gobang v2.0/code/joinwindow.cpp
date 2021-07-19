#include "joinwindow.h"
#include "ui_joinwindow.h"

#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"

JoinWindow::JoinWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoinWindow)
{
    ui->setupUi(this);
    ui->groupBox->setStyleSheet("QGroupBox{border:none}");
    ui->groupBox_2->setStyleSheet("QGroupBox{border:none}");
    ui->groupBox_3->setStyleSheet("QGroupBox{border:none}");
    ui->groupBox_4->setStyleSheet("QGroupBox{border:none}");

    setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);

    setWindowTitle("Join Room");

    setWindowIcon(QIcon(":/res/window.jpg"));

    netWorkGameClient = new NetWorkGameClient;
    connect(netWorkGameClient, &NetWorkGameClient::successConnect, this, [=]{

        isConnect  = 1;
        goToRoom();
    });
}

JoinWindow::~JoinWindow()
{
    delete ui;
}

void JoinWindow::on_pushButton_clicked()
{

    netWorkGameClient->ip = ui->ip->text();
    netWorkGameClient->port = ui->port->text().toInt();
    netWorkGameClient->startConnect();

    QTimer::singleShot(1000, this, [=]{
        if(isConnect == 0)
        {
            QMessageBox::information(this, "Hi", "connection failed");
            ui->ip->clear();
            ui->port->clear();
            return ;
        }
    });


//
} //127.0.0.1


void JoinWindow::goToRoom()
{
    QMessageBox::information(this, "Hi", "connection successed");
    netWorkGameClient->mainSceneClient->setGeometry(this->geometry());
    this->close();
    netWorkGameClient->mainSceneClient->show();

}
void JoinWindow::on_pushButton_2_clicked()
{

    MainWindow *nextMainWindow = new MainWindow;
    nextMainWindow->setGeometry(this->geometry());
    this->close();
    nextMainWindow->show();
}
void JoinWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/bg.png");
    painter.drawPixmap(-300,-300,1500,1500,pix);
}
