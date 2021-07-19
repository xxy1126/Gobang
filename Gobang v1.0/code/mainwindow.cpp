#include "mainwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui->setupUi(this);

    //设置固定大小
    setFixedSize(MARGIN*5 + BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2 + BLOCK_SIZE*BOARD_GRAD_SIZE);

    //设置图标
    setWindowIcon(QIcon(":/res/window.jpg"));

    //设置标题
    setWindowTitle("五子棋游戏");


    initGame();


}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing,true); // 抗锯齿

    for (int i = 1; i<BOARD_GRAD_SIZE + 1; i++)
    {
        //从左到右，第i+1条竖线
        painter.drawLine(MARGIN + BLOCK_SIZE*i, MARGIN + BLOCK_SIZE,
                         MARGIN + BLOCK_SIZE*i, size().height()-MARGIN);
        //从上到下，第i+1条横线
        painter.drawLine(MARGIN + BLOCK_SIZE, MARGIN + BLOCK_SIZE*i,
                         size().width()-MARGIN*4, MARGIN + BLOCK_SIZE*i);
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

    //绘制棋子
    for(int i = 0; i < BOARD_GRAD_SIZE; i++)
        for(int j = 0; j < BOARD_GRAD_SIZE; j++)
        {
            if(game->gameMapVec[i][j] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN + BLOCK_SIZE * j - CHESS_RADIUS, MARGIN + BLOCK_SIZE * i - CHESS_RADIUS,
                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
            }
            else if(game->gameMapVec[i][j] == -1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN + BLOCK_SIZE * j - CHESS_RADIUS, MARGIN + BLOCK_SIZE * i - CHESS_RADIUS,
                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
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
            game->gamestatus = WIN;
            //QSound::play(":sound/win.wav");
            QString str;
            if(game->gameMapVec[clickPosRow][clickPosCol] == 1)
            {
                str = "黑棋";
            }
            else if(game->gameMapVec[clickPosRow][clickPosCol] == -1)
            {
                str = "白棋";
            }
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "五子棋决战", str + "胜利");

            //重置游戏状态，否则容易死循环
            if(btnValue == QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gamestatus = PLAYING;
            }
        }
    }
}

void MainWindow::initGame()
{
    //初始化游戏模型
    game = new GameModel;
    initAIGame();

}

void MainWindow::initAIGame()
{
    game_type = AI;
    game->gamestatus = PLAYING;

    //在数据模型中进行初始化功能
    game->startGame(game_type);
    update();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
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
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(selectPos == false)
        return;
    else
        selectPos = false;

    chessOneByPerson();

    if(game_type == AI)
    {
        //AI 下棋
        QTimer::singleShot(AI_THINK_TIME, this, SLOT(chessOneByAI()));
    }
}

void MainWindow::chessOneByPerson()
{
    //根据当前存储的坐标下子
    //只有有效点击才下子，并且该处没有子
    if(clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        //在数据模型中落子
        game->actionByPerson(clickPosRow,clickPosCol);
        //播放落子音效

        //重绘
        update();
    }
}

void MainWindow::chessOneByAI()
 {
     game->actionByAI(clickPosRow,clickPosCol);
     //QSound::play(":sound/chessone.wav");
     update();
 }
