#include "mainwindow.h"
#include "choosescene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);

    setWindowTitle("网络对战");

    setWindowIcon(QIcon(":/res/window.jpg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonCreate_clicked()
{
    roomWindow = new RoomWindow(NULL);
    roomWindow->setGeometry(this->geometry());
    this->close();
    roomWindow->show();
}


void MainWindow::on_pushButtonJoin_clicked()
{
    joinWindow = new JoinWindow(NULL);
    joinWindow->setGeometry(this->geometry());
    this->close();
    joinWindow->show();
}


void MainWindow::on_pushButtonReturn_clicked()
{
    Choosescene *nextChoosescene = new Choosescene;
    nextChoosescene->setGeometry(this->geometry());
    this->close();
    nextChoosescene->show();
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/bg.png");
    painter.drawPixmap(-300,-300,1500,1500,pix);
}

