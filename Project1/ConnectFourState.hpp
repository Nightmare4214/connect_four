#pragma once
#include<iostream>
using namespace std;
class ConnectFourState {
private:
	//移动到col列最高的位置
	static uint64_t topMask(const int & col);
	//移动到col列最底下
	static uint64_t bottomMask(const int & col);
	//取整列
	static uint64_t columnMask(const int & col);
	//自己的棋子
	uint64_t board;
	//所有的棋子
	uint64_t mask;
	
public:
	//最后一步
	int lastMove;
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
	void init(const string &s) {
		for (const char &c : s)play(c - '0');
	}
	/**
	 * 判断这一步下下去能不能赢
	 * @param col 放在第几列(0-WIDTH-1)
	 * @param opponentFlag 是对手
	 * @param predict 这一步还没下
	 * @return  判断这一步下下去能不能赢
	 */
	bool isWinMove(const int &col, const bool &opponentFlag, const bool& predict = false)const;
	//输出棋盘
	void printBoard()const;
	/**
	 * 获得游戏结果
	 * @return col X胜利则为1，平局为0，O胜利则为-1,游戏没结束返回-2
	 */
	int getResult()const;
	/**
	 * 判断游戏是否结束
	 * @return 判断游戏是否结束
	 */
	bool isTerminal()const;
};