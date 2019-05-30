#pragma once
#include<iostream>
using namespace std;
class ConnectFourState {
private:
	//移动到col列最高的位置
	static uint64_t topMask(const int &col);
	//移动到col列最底下
	static uint64_t bottomMask(const int &col);
	//自己的棋子
	uint64_t board;
	//所有的棋子
	uint64_t mask;
	//最后一步
	int lastMove;
public:
	static const int WIDTH = 8;
	static const int HEIGHT = 6;
	//回合数
	int round;
	ConnectFourState(const uint64_t& board = 0,const uint64_t& mask = 0, const int& round = 0);
	ConnectFourState(const int& col, const uint64_t& board = 0, const uint64_t& mask = 0, const int& round = 0);
	//获得当前的玩家
	int getPlayer() const;
	//这一列是否能下
	bool canPlay(const int &col)const;
	/**
	 * 放棋子
	 * @param col 放在第几列(0-WIDTH-1)
	 */
	void play(const int &col);
	/**
	 * 判断这一步下下去能不能赢
	 * @param col 放在第几列(0-WIDTH-1)
	 * @param opponentFlag 是对手
	 * @return  判断这一步下下去能不能赢
	 */
	bool isWinMove(const int &col,const bool &opponentFlag);
	//输出棋盘
	void printBoard()const;
	/**
	 * 获得游戏结果
	 * @return col X胜利则为1，平局为0，O胜利则为-1,游戏没结束返回-2
	 */
	int getResult();
	/**
	 * 判断游戏是否结束
	 * @return 判断游戏是否结束
	 */
	bool isTerminal();
};