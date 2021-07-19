#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamemodel.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GameModel *game; //游戏指针
    GameType game_type;

    int clickPosRow,clickPosCol; //存储将点击的位置
    bool selectPos = false; //是否移动到合适的位置，已选中某个交叉点

    //绘制
    void paintEvent(QPaintEvent *event);

    void initGame();

    void initAIGame();

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void chessOneByPerson();

private slots:
    void chessOneByAI();
};
#endif // MAINWINDOW_H
