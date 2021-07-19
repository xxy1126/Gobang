#include "gamemodel.h"
#include <time.h>
#include <stdlib.h>

GameModel::GameModel()
{

}

void GameModel::startGame(GameType type)
{
    gametype = type;

    num = 0;

    posRecord.clear();

    gameMapVec.clear();
    for(int i = 0; i<BOARD_GRAD_SIZE; i++)
    {
        std::vector<int> lineBoard;
        for(int j = 0; j<BOARD_GRAD_SIZE; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }

    //如果是AI模式，需要初始化评分数组
    if(gametype == AI)
    {
        scoreMapVec.clear();
        for(int i = 0; i<BOARD_GRAD_SIZE; i++)
        {
            std::vector<int> lineBoard;
            for(int j = 0; j<BOARD_GRAD_SIZE; j++)
                lineBoard.push_back(0);
           scoreMapVec.push_back(lineBoard);
        }
    }

     //轮到黑棋下棋
     playerFlag = true;
}

void GameModel::actionByPerson(int row,int col)
{
    updateGameMap(row,col);
}

void GameModel::updateGameMap(int row,int col)
{
    if(playerFlag)
        gameMapVec[row][col] = 1;
    else
        gameMapVec[row][col] = -1;

    //换手
    playerFlag = !playerFlag;
}

bool GameModel::isWin(int row,int col)
{
    // 水平方向
    for(int i = 0; i < 5; i++)
    {
        if(col - i > 0 &&
                col - i + 4 < BOARD_GRAD_SIZE &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // 竖直方向
    for(int i = 0; i < 5; i++)
    {
        if(row - i + 4< BOARD_GRAD_SIZE &&
                row - i> 0 &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // “/”方向
    for(int i = 0; i < 5; i++)
    {
        if(row + i  < BOARD_GRAD_SIZE && row + i - 4 > 0 && col - i > 0 &&
                col - i +4 < BOARD_GRAD_SIZE &&
                gameMapVec[row + i][col - i] == gameMapVec[row + i -1][col - i + 1] &&
                gameMapVec[row + i][col - i] == gameMapVec[row + i -2][col - i + 2] &&
                gameMapVec[row + i][col - i] == gameMapVec[row + i -3][col - i + 3] &&
                gameMapVec[row + i][col - i] == gameMapVec[row + i -4][col - i + 4])
            return true;
    }

    // “\”方向
    for(int i = 0; i < 5; i++)
    {
        if(row - i + 4 < BOARD_GRAD_SIZE &&
                row - i > 0 &&
                col - i > 0 &&
                col - i +4 < BOARD_GRAD_SIZE &&
                gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
                gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
                gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
                gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }
    return false;
}

void GameModel::calculateScore()
{
    //统计玩家或者电脑连成的子
    int personNum = 0;// 玩家连成子的个数
    int botNum = 0;//人机连成子的个数
    int emptyNum = 0;// 各个方向空白位的个数

    //清空评分数组
    scoreMapVec.clear();
    for(int i = 0; i < BOARD_GRAD_SIZE; i++)
    {
        std::vector<int> lineScores;
        for(int j = 0; j < BOARD_GRAD_SIZE; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    //计分(完全遍历)
    for(int row = 0; row < BOARD_GRAD_SIZE; row++)
        for(int col = 0; col < BOARD_GRAD_SIZE; col++)
        {
            if(row > 0 && col > 0 && gameMapVec[row][col] == 0)//从空白点计算
            {
                for(int y = -1; y <= 1; y++)//遍历八个方向
                    for(int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum=0;
                        botNum=0;
                        emptyNum=0;

                        //原坐标不算
                        if(!(y == 0 && x == 0))
                        {
                            //每个方向延伸四个棋子
                            //对玩家白子评分（正反两个方向）
                            //正方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row + i*y > 0 && row + i*y < BOARD_GRAD_SIZE &&
                                        col + i*x > 0 && col + i*x <BOARD_GRAD_SIZE &&
                                        gameMapVec[row + i*y][col + i*x] == 1)// 真人玩家的子
                                {
                                    personNum++;
                                }
                                else if(row + i*y >0  && row + i*y < BOARD_GRAD_SIZE
                                        && col + i*x >0 && col + i*x < BOARD_GRAD_SIZE&&
                                        gameMapVec[row + i*y][col + i*x]==0)//  空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界或有人机棋子
                                    break;
                            }

                            //反方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row - i*y > 0 && row- i*y <BOARD_GRAD_SIZE
                                        && col- i*x > 0 && col - i*x < BOARD_GRAD_SIZE &&
                                        gameMapVec[row - i*y][col - i*x]==1)
                                {
                                    personNum++;
                                }
                                else if(row - i*y >0 && row - i*y < BOARD_GRAD_SIZE
                                        && col- i*x > 0 && col- i*x <BOARD_GRAD_SIZE &&
                                        gameMapVec[row - i*y][col - i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界或有人机棋子
                                    break;
                            }

                            //计分
                            if(personNum == 1)   // 杀一
                                scoreMapVec[row][col]+=10;
                            else if(personNum == 2) // 杀二
                            {
                                if(emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if(personNum == 3)// 杀四
                            {
                                //量变位不一样，优先级不一样
                                if(emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=110;
                            }
                            else if(personNum == 4) // 杀五
                                scoreMapVec[row][col]+=12000;

                            //进行一次清空
                            emptyNum = 0;

                            //对人机白子评分
                            //正方向
                            for(int i=1;i<=4;i++)
                            {
                                if(row + i*y > 0 && row + i*y < BOARD_GRAD_SIZE
                                        && col + i*x >0 && col + i*x <BOARD_GRAD_SIZE &&
                                        gameMapVec[row + i*y][col + i*x] == -1)//真人玩家的子
                                {
                                    botNum++;
                                }
                                else if(row + i*y >0 && row + i*y < BOARD_GRAD_SIZE
                                        && col + i*x >0 && col + i*x < BOARD_GRAD_SIZE &&
                                        gameMapVec[row + i*y][col + i*x]==0)//空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else //出边界
                                    break;
                            }

                            // 反方向
                            for(int i = 1; i <= 4; i++)
                            {
                                if(row - i*y > 0 && row - i*y < BOARD_GRAD_SIZE
                                        && col- i*x > 0 && col - i*x < BOARD_GRAD_SIZE &&
                                        gameMapVec[row - i*y][col - i*x] == 1)
                                {
                                    botNum++;
                                }
                                else if(row - i*y >0 && row- i*y < BOARD_GRAD_SIZE
                                        && col-i*x>0 && col-i*x < BOARD_GRAD_SIZE &&
                                        gameMapVec[row - i*y][col - i*x]==0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else // 出边界
                                    break;
                            }

                            //计分
                            if(botNum == 0)                         // 普通
                                scoreMapVec[row][col]+=5;
                            else if(botNum == 1)                    // 活二
                                scoreMapVec[row][col]+=10;
                            else if(botNum == 2)
                            {
                                if(emptyNum == 1)                   //  死三
                                    scoreMapVec[row][col]+=25;
                                else if(emptyNum == 2)              // 活三
                                    scoreMapVec[row][col]+=50;
                            }
                            else if(botNum == 3)
                            {
                                if(emptyNum == 1)                    //死四
                                    scoreMapVec[row][col] += 55;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=100;     // 活四
                            }
                            else if(botNum >= 4)                  //活五
                                scoreMapVec[row][col] += 120000;
                        }
                    }
            }
        }
}

 void GameModel::actionByAI(int &clickRow,int &clickCol)
 {
     //计算评分
     calculateScore();

     //从评分中找出最大分数的位置
     int maxScore = 0;
     std::vector<std::pair<int,int>> maxPoints;

     for(int row = 1; row < BOARD_GRAD_SIZE; row++)
     {
         for(int col = 1; col < BOARD_GRAD_SIZE ; col++)
         {
             //前提是坐标为空
             if(gameMapVec[row][col] == 0)
             {
                 if(scoreMapVec[row][col] > maxScore)
                 {
                     maxPoints.clear();
                     maxScore = scoreMapVec[row][col];
                     maxPoints.push_back(std::make_pair(row,col));
                 }
                 else if(scoreMapVec[row][col] == maxScore)
                     maxPoints.push_back(std::make_pair(row,col));
             }
         }
     }
     //随机落子
     srand((unsigned)time(0));
     int index = rand() % maxPoints.size();

     std::pair<int, int> pointPair = maxPoints.at(index);
     clickRow = pointPair.first; //记录落子点
     clickCol = pointPair.second;
     updateGameMap(clickRow,clickCol);
 }


