#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ---五子棋游戏模型类--- //
#include <vector>

// 游戏类型
enum GameType
{
    MAN,   //人人
    AI     //人机
};

// 游戏状态
enum GameStatus
{
    PLAYING,
    OVER

};

// 棋盘尺寸
const int BOARD_GRAD_SIZE = 16;

const int MARGIN = 30; //棋盘边缘空隙
const int CHESS_RADIUS = 15; //棋子半径
const int MARK_SIZE = 6; // 落子标记边长
const int BLOCK_SIZE = 40; //格子大小
const int POS_OFFSET = BLOCK_SIZE*0.4; //鼠标点击的模糊距离上限

const int AI_THINK_TIME = 500; // AI下棋的思考时间

class GameModel
{
public:
    GameModel();

public:
    //存储当前游戏棋盘和棋子的情况，空白为0，黑子为1，白子-1
    std::vector<std::vector<int>> gameMapVec;

    //存储各个点位的评分情况，作为AI下棋依据
    std::vector<std::vector<int>> scoreMapVec;

    //标示下棋方，true：黑棋  false：白棋
    bool playerFlag;

    GameType gametype; //游戏模式

    GameStatus gamestatus; //游戏状态

    //存储下棋顺序
    std::vector<std::pair<int,int>> posRecord;

    //所下棋子数
    int num;

    void startGame(GameType type); //开始游戏
    void calculateScore(); //计算评分
    void actionByPerson(int row,int col); //人执行下棋
    void actionByAI(int &clickRow,int &clickCol); //机器执行下棋
    void updateGameMap(int row,int col); //每次落子后更新游戏棋盘
    bool isWin(int row,int col); //判断游戏是否胜利
    bool isDeadGame(); //判断是否和棋

};

#endif // GAMEMODEL_H
