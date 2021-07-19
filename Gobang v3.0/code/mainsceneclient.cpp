#include "mainsceneclient.h"
#include "choosescene.h"

#include <math.h>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QTimer>
#include <QFont>
#include <QDebug>


MainSceneClient::MainSceneClient(QWidget *parent) : QMainWindow(parent)
{
    //鼠标追踪事件
    this->setMouseTracking(true);

    //设置固定大小
    this->setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);

    //设置图标
    this->setWindowIcon(QIcon(":/res/window.jpg"));

    //设置标题
    this->setWindowTitle("五子棋游戏");

    //创建开始菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //创建返回菜单项
    QAction * backAction = startMenu->addAction("返回");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //点击返回 实现返回上个界面
    connect(backAction,&QAction::triggered,[=](){
        //延时返回
        QTimer::singleShot(300,this,[=](){
            this->ifEndGame();
        });
    });

    //认输按钮
    QPushButton * giveupBtn = new QPushButton(this);

    giveupBtn->setText("认输");
    //giveupBtn->setFlat(true);
    giveupBtn->setStyleSheet("background:transparent;border:2px solid white;");
    giveupBtn->setFont(QFont("华文行楷",15));

    giveupBtn->move(this->width()-giveupBtn->width()*1.2,this->height()*0.3);

    connect(giveupBtn,&QPushButton::clicked,this,[=](){
        this->giveup();
    });

    //悔棋按钮
    QPushButton * repentanceBtn = new QPushButton(this);

    repentanceBtn->setText("悔棋");
    repentanceBtn->setStyleSheet("background:transparent;border:2px solid white;");
    repentanceBtn->setFont(QFont("华文行楷",15));

   repentanceBtn->move(this->width()-repentanceBtn->width()*1.2,this->height()*0.5);

    connect(repentanceBtn,&QPushButton::clicked,this,[=](){
        this->repentance();
    });

    //显示/隐藏顺序按钮
    QPushButton * orderBtn = new QPushButton(this);

    orderBtn->setText("显示棋子顺序");
    orderBtn->setStyleSheet("background:transparent;border:2px solid white;");
    orderBtn->setFont(QFont("华文行楷",10));

    orderBtn->move(this->width()-orderBtn->width()*1.2,this->height()*0.7);

    connect(orderBtn,&QPushButton::clicked,this,[=](){
        flag = !flag;
        if(flag)
            orderBtn->setText("隐藏棋子顺序");
        else
            orderBtn->setText("显示棋子顺序");
    }
    );

    //激活游戏
    initGame();

}

void MainSceneClient::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing,true); // 抗锯齿

    //设置背景图片
    QPixmap pix;
    pix.load(":/res/lightwood.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //棋盘
    for (int i = 1; i<BOARD_GRAD_SIZE; i++)
    {
        //从左到右，第i+1条竖线
        painter.drawLine(MARGIN + BLOCK_SIZE*i, MARGIN + BLOCK_SIZE,
                         MARGIN + BLOCK_SIZE*i, size().height()-MARGIN - BLOCK_SIZE);
        //从上到下，第i+1条横线
        painter.drawLine(MARGIN + BLOCK_SIZE, MARGIN + BLOCK_SIZE*i,
                         size().width()-MARGIN*4 - BLOCK_SIZE, MARGIN + BLOCK_SIZE*i);
    }

    //绘制选中点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    //绘制落子标记（防止鼠标出框越界）
    if(clickPosRow > 0 && clickPosRow < BOARD_GRAD_SIZE &&
            clickPosCol > 0 && clickPosCol < BOARD_GRAD_SIZE &&
            game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        if(game->playerFlag)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(MARGIN + BLOCK_SIZE * clickPosCol - MARK_SIZE / 2,
                         MARGIN + BLOCK_SIZE * clickPosRow - MARK_SIZE / 2,
                         MARK_SIZE,MARK_SIZE);
    }

    //天元
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawEllipse(MARGIN + BLOCK_SIZE * 4  - CHESS_RADIUS * 0.5, MARGIN + BLOCK_SIZE * 4  - CHESS_RADIUS * 0.5,
                        CHESS_RADIUS,CHESS_RADIUS);
    painter.drawEllipse(MARGIN + BLOCK_SIZE * 12  - CHESS_RADIUS * 0.5, MARGIN + BLOCK_SIZE * 4  - CHESS_RADIUS * 0.5,
                        CHESS_RADIUS,CHESS_RADIUS);
    painter.drawEllipse(MARGIN + BLOCK_SIZE * 8 - CHESS_RADIUS * 0.5, MARGIN + BLOCK_SIZE * 8 - CHESS_RADIUS * 0.5,
                        CHESS_RADIUS,CHESS_RADIUS);
    painter.drawEllipse(MARGIN + BLOCK_SIZE * 4 - CHESS_RADIUS * 0.5, MARGIN + BLOCK_SIZE * 12 - CHESS_RADIUS * 0.5,
                        CHESS_RADIUS,CHESS_RADIUS);
    painter.drawEllipse(MARGIN + BLOCK_SIZE * 12 - CHESS_RADIUS * 0.5, MARGIN + BLOCK_SIZE * 12 - CHESS_RADIUS * 0.5,
                        CHESS_RADIUS,CHESS_RADIUS);

    //绘制棋子
    for(auto ptr = game->posRecord.begin(); ptr != game->posRecord.end(); ptr++)
    {

        if(game->gameMapVec[(*ptr).first][(*ptr).second] == 1)
            brush.setColor(Qt::black);
        else if(game->gameMapVec[(*ptr).first][(*ptr).second] == -1)
            brush.setColor(Qt::white);

        painter.setBrush(brush);
        painter.drawEllipse(MARGIN + BLOCK_SIZE * (*ptr).second - CHESS_RADIUS, MARGIN + BLOCK_SIZE * (*ptr).first - CHESS_RADIUS,
                            CHESS_RADIUS*2,CHESS_RADIUS*2);
        if(flag)
        {
            //绘制数字
            painter.setBrush(Qt::NoBrush);
            QFont font("宋体",10,QFont::Bold,true);
            painter.setFont(font);
            if(game->gameMapVec[(*ptr).first][(*ptr).second] == -1)
                painter.setPen(Qt::black);
            else
                painter.setPen(Qt::white);
            QString strnum = QString::number(ptr - game->posRecord.begin() + 1);
            painter.drawText(MARGIN + BLOCK_SIZE * (*ptr).second  - 10,MARGIN + BLOCK_SIZE * (*ptr).first + 5, strnum);
        }
    }

    // 判断输赢
    if(clickPosRow > 0 && clickPosRow < BOARD_GRAD_SIZE &&
            clickPosCol > 0 && clickPosCol < BOARD_GRAD_SIZE &&
            (game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
             game->gameMapVec[clickPosRow][clickPosCol] == -1))
    {
        if(game->isWin(clickPosRow,clickPosCol) && game->gamestatus == PLAYING)
        {
            game->gamestatus = OVER;

            QString str;
            if(game->gameMapVec[clickPosRow][clickPosCol] == 1)
                str = "黑棋";
            else if(game->gameMapVec[clickPosRow][clickPosCol] == -1)
                str = "白棋";

            QMessageBox::information(this, "五子棋决战", str + "胜利");

            //重置游戏状态，否则容易死循环
            if(QMessageBox::Yes ==  QMessageBox::question(this,"question","Continue?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
            {
                game->startGame(game_type);
                game->gamestatus = PLAYING;
            }
            else
                this->close();
        }
     }
}

void MainSceneClient::initGame()
{
    //初始化游戏模型
    game = new GameModel;

    game->gamestatus = PLAYING;

    //在数据模型中进行初始化功能
    game->startGame(game_type);

    update();

}


void MainSceneClient::mouseMoveEvent(QMouseEvent *event)
{
    //通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    //棋盘边缘不能落子
    if(x > MARGIN + BLOCK_SIZE/2 &&
            x < size().width() - MARGIN*4 - BLOCK_SIZE /2 &&
            y >= MARGIN +BLOCK_SIZE/2 &&
            y < size().height() - MARGIN - BLOCK_SIZE /2)
    {
        //获取最近的左上角的点
        // add by rock
        int col = (x - MARGIN) / BLOCK_SIZE;
        int row = (y - MARGIN) / BLOCK_SIZE;

        int leftTopPosX = MARGIN + BLOCK_SIZE * col;
        int leftTopPosY = MARGIN + BLOCK_SIZE * row;

        //根据距离计算出合适的点击位置，一共四个点，根据半径距离选最近的
        clickPosRow = -1;
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        selectPos = false;

        //确定一个误差在范围之内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if(len < POS_OFFSET)
        {
            clickPosRow = row;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos = true;
            }
        }

        len = sqrt((x - leftTopPosX - BLOCK_SIZE) * (x - leftTopPosX - BLOCK_SIZE) + (y - leftTopPosY) * (y - leftTopPosY));
        if(len < POS_OFFSET)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos = true;
            }
        }

        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BLOCK_SIZE) * (y - leftTopPosY - BLOCK_SIZE));
        if(len < POS_OFFSET)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos = true;
            }
        }

        len = sqrt((x - leftTopPosX - BLOCK_SIZE) * (x - leftTopPosX - BLOCK_SIZE) + (y - leftTopPosY - BLOCK_SIZE) * (y - leftTopPosY - BLOCK_SIZE));
        if(len < POS_OFFSET)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos = true;
            }
        }

    }

    update();


    //return QMainWindow::mouseMoveEvent(event);
}


void MainSceneClient::mouseReleaseEvent(QMouseEvent *)
{
    if(selectPos == false)
        return;
    else
        selectPos = false;
    if(turnToRemote == 1)
        return ;
    chessOneByPerson();

    update();

}
void MainSceneClient::chessOneByOther()
{
    if(clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        turnToRemote ^= 1;
        game->actionByPerson(clickPosRow,clickPosCol);

        game->posRecord.push_back(std::make_pair(clickPosRow,clickPosCol));
        game->num++;

        update();
    }
    update();
}
void MainSceneClient::chessOneByPerson()
{
    //根据当前存储的坐标下子
    //只有有效点击,并且该处没有子才下子
    if(clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        //在数据模型中落子
        turnToRemote ^= 1;
        game->actionByPerson(clickPosRow,clickPosCol);

        game->posRecord.push_back(std::make_pair(clickPosRow,clickPosCol));
        game->num++;

        //重绘
        update();

        emit netWorkSendMessage();
    }
}

void MainSceneClient::ifEndGame()
{
    if(QMessageBox::Yes == QMessageBox::question(this,"quit","Quit the game?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        Choosescene *nextChooseScene;
        nextChooseScene = new Choosescene;
        this->hide();
        nextChooseScene->setGeometry(this->geometry());
        nextChooseScene->show();
    }
    else
        return;
}

void MainSceneClient::repentance()
{

    if(game->num >0)
    {
        if(turnToRemote == 1)
        {
            //sendsingal
            emit sendregret();
        }
        else
        {
            //error
            QMessageBox::information(this, "hi", "你不能进行悔棋");
        }
    }

    update();
}
void MainSceneClient::repentanceByEnemy()
{
    if(QMessageBox::Yes == QMessageBox::question(this,"hi","是否允许对方悔棋",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        if(game->num >0)
        {
            turnToRemote ^= 1;
            game->gameMapVec[game->posRecord[game->num-1].first][game->posRecord[game->num-1].second] = 0;
            game->num--;
            game->posRecord.pop_back();
            game->playerFlag = !game->playerFlag;
        }
        update();
        emit sendYesMessage();
    }
    else
    {
        emit sendNoMessage();
    }
}
void MainSceneClient::yesRegret()
{
    QMessageBox::information(this, "hi", "对方同意悔棋");
    turnToRemote = 0;
    game->gameMapVec[game->posRecord[game->num-1].first][game->posRecord[game->num-1].second] = 0;
    game->num--;
    game->posRecord.pop_back();
    game->playerFlag = !game->playerFlag;
}
void MainSceneClient::noRegret()
{
    QMessageBox::information(this, "hi", "对方不同意悔棋");
}


void MainSceneClient::giveup()
{
    game->gamestatus = OVER;



    QMessageBox::information(this, "五子棋决战", "对方获得胜利");

    emit sendGiveUp();

    if(QMessageBox::Yes == QMessageBox::question(this,"question","continue?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        //重置游戏状态
        turnToRemote = 1;
        game->startGame(game_type);
        game->gamestatus = PLAYING;
    }
    else
        this->close();
}
void MainSceneClient::giveUpByEnemy()
{
    game->gamestatus = OVER;

    QMessageBox::information(this, "hi", "对方认输,你获得了胜利" );

    if(QMessageBox::Yes == QMessageBox::question(this,"question","continue?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        //重置游戏状态
        turnToRemote = 1;
        game->startGame(game_type);
        game->gamestatus = PLAYING;
    }
    else
        this->close();
}

