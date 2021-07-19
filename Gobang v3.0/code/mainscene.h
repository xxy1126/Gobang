#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "gamemodel.h"

#include <QMainWindow>

class Mainscene : public QMainWindow
{
    Q_OBJECT
public:
    explicit Mainscene(QWidget *parent = nullptr);

    GameModel *game; //游戏指针
    GameType game_type;

    int clickPosRow,clickPosCol; //存储将点击的位置
    bool selectPos = false; //是否移动到合适的位置，已选中某个交叉点

    bool flag = false; //true表示显示顺序，false表示隐藏

    bool flag_repentence = false; //是否已悔棋

    bool flag_ai = false; // AI是否已经下过棋

    bool flag_man = false ; //限制玩家下第一步

    //绘制
    void paintEvent(QPaintEvent *event);

    void initGame();

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void chessOneByPerson();

    void ifEndGame();

    void repentance();

    void giveup();

    void orderchange(bool flag);

private slots:
    void chessOneByAI();

signals:
    //写一个自定义信号 告诉主场景 点击了返回
    void chooseSceneBack();


};

#endif // MAINSCENE_H
