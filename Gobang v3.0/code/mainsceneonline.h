#ifndef MAINSCENEONLINE_H
#define MAINSCENEONLINE_H

#include <QMainWindow>
#include <QLabel>
#include "gamemodel.h"



class MainSceneOnline : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainSceneOnline(QWidget *parent = nullptr);

    GameModel *game; //游戏指针
    GameType game_type;
    QLabel *connectLabel;
    int clickPosRow,clickPosCol; //存储将点击的位置
    bool selectPos = false; //是否移动到合适的位置，已选中某个交叉点

    bool flag = false; //true表示显示顺序，false表示隐藏
    bool turnToRemote = false;
    bool connectSuccess = 0;
    //绘制
    void paintEvent(QPaintEvent *event);

    void initGame();

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void chessOneByPerson();
    void chessOneByOther();

    void ifEndGame();

    void repentance();
    void repentanceByEnemy();
    void yesRegret();
    void noRegret();

    void giveup();
    void giveUpByEnemy();

    void orderchange(bool flag);

signals:
    //写一个自定义信号 告诉主场景 点击了返回
    void chooseSceneBack();

    //network send message
    void netWorkSendMessage();
    void sendGiveUp();
    void sendregret();
    void sendYesMessage();
    void sendNoMessage();
};

#endif

