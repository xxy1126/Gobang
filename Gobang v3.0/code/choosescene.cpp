#include "choosescene.h"
#include "ui_choosescene.h"
#include "mypushbotton.h"
#include "mainscene.h"
#include "gamemodel.h"
#include "mainsceneclient.h"
#include "mainsceneonline.h"

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

Choosescene::Choosescene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置固定大小
    setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);

    //设置图标
    setWindowIcon(QIcon(":/res/window.jpg"));

    //设置标题
    setWindowTitle("五子棋游戏");

    //pvp按钮
    MyPushBotton * pvpBtn = new MyPushBotton(":/res/pvp.png");
    pvpBtn->setParent(this);
    pvpBtn->move(this->width()*0.5-pvpBtn->width()*0.5,10);

    //pvc按钮
    MyPushBotton * pvcBtn = new MyPushBotton(":/res/pve.png");
    pvcBtn->setParent(this);
    pvcBtn->move(this->width()*0.5-pvcBtn->width()*0.5,100);


    //net
    MyPushBotton * netBtn = new MyPushBotton(":/res/net.png");
    netBtn->setParent(this);
    netBtn->move(this->width()*0.5-pvcBtn->width()*0.5,190);

    //实例化下棋场景
    scene = new Mainscene;

    //监听主界面的返回按钮发送的信号
    connect(scene,&Mainscene::chooseSceneBack,this,[=](){
        scene->hide();//隐藏主界面
        //位置一致
        this->setGeometry(scene->geometry());
        this->show();//重新显示主场景
    });

    connect(pvpBtn,&MyPushBotton::clicked,[=](){
        pvpBtn->zoomup();
        pvpBtn->zoomdown();

        //延时进入下棋界面
        QTimer::singleShot(500,this,[=](){
            //进入到下棋界面
            //自身隐藏
            this->hide();
            scene->setGeometry(this->geometry());
            //显示下棋场景
            scene->show();
            //位置一致
            scene->setGeometry(this->geometry());
            //选择pvp
            scene->game_type = MAN;
        });

    });

    connect(pvcBtn,&MyPushBotton::clicked,[=](){
        pvcBtn->zoomup();
        pvcBtn->zoomdown();

        //延时进入下棋界面
        QTimer::singleShot(500,this,[=](){
            //进入到下棋界面
            //自身隐藏
            this->hide();
            //位置一致
            scene->setGeometry(this->geometry());
            //显示下棋场景
            scene->show();

            //选择pvc
            scene->game_type = AI;

        });
    });

    connect(netBtn,&MyPushBotton::clicked,[=](){
        netBtn->zoomup();
        netBtn->zoomdown();

        //延时进入下棋界面
        QTimer::singleShot(500,this,[=](){

            this->hide();
            mainWindow = new MainWindow;
            mainWindow->setGeometry(this->geometry());
            mainWindow->show();
        });
    });
}

Choosescene::~Choosescene()
{
    delete ui;
}

void Choosescene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/scene.jpeg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
