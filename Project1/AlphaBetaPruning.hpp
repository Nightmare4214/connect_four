#pragma once
#pragma comment(lib,"ws2_32.lib")
#include"TranspositionTable.hpp"
//libwsock32.a
#include<Winsock2.h>
#include<Ws2tcpip.h>
#include<cstdio>
#include<iostream>
#include<string>
using namespace std;
class AlphaBetaPruning {
	//高和宽
	static const int HEIGHT = 6, WIDTH = 8;
	//最大深度
	static const int maxDepth = 12;

	//服务端ip
	const char *IP = "127.0.0.1";
	//服务端的端口
	const int PORT = 6000;
	//队名
	const string TeamName = "Nightmare_alpha_beta_pruning_v2";

	TranspositionTable table;

	//无穷大
	static const int INF = 0x3f3f3f3f;
	//棋盘,0代表空_，1代表白(先手)X，-1代表黑O
	int board[WIDTH][HEIGHT];
	//当前每个列的高度
	int currentHeight[WIDTH];
	//第几回合
	int round;
	//下棋顺序
	int columnOrder[WIDTH];
	/*位棋盘，只存当前玩家的棋子
	HEIGHT+1位存1个列(最高的用来表示满了)
	通过^掩码来改变当前玩家的棋子
	*/
	uint64_t bitBoard;
	/*
	掩码,存所有的棋子，用1表示
	通过在列的最底下+1后或运算来下棋
	*/
	uint64_t mask;

	//移动到col列最高的位置
	static uint64_t topMask(const int& col);

	//移动到col列最底下
	static uint64_t bottomMask(const int& col);

	//取整列
	static uint64_t columnMask(const int & col);

	//底部全1
	static uint64_t allBottomMask();

	uint64_t getKey()const;

	/**
	 * 获得玩家输入的正确的列
	 * @return 正确的列
	 */
	int getCorrectCol()const;
	/**
	 * 评估局面得分：规则为所有的空位置放上棋子后有几种赢法
	 * @param player 1代表X,-1代表O
	 * @return 得分
	 */
	int evaluate(const int& player)const;
	/**
	 * alpha-beta剪枝
	 * @param depth 搜索深度
	 * @param alpha
	 * @param beta
	 * @param player 玩家，1代表X,-1代表O
	 * @param move 下的列
	 * @return alpha值
	 */
	int alphaBetaPruning(int depth, int alpha, int beta, int player, int& move);

	int getColByAlphaBetaPruning(const int& player);
	/**
	 * 初始化socket
	 */
	void initSocket();
	/**
	 * 关闭socket
	 */
	void closeSocket();
	/**
	 * 发送消息
	 * @param msg 发送的消息
	 */
	void sendData(const string& msg)const;
	/**
	 * 接收消息
	 * @return 接收到的消息
	 */
	string recvData()const;
public:
	AlphaBetaPruning();
	/**
	 * 输出棋盘
	 */
	void printBoard()const;
	/**
	 * 这一列能不能放棋子
	 * @param col 放在第几列(0-WIDTH-1)
	 * @return 这一列能不能放棋子
	 */
	bool canPlay(const int& col)const;
	/**
	 * 放棋子
	 * @param col 放在第几列(0-WIDTH-1)
	 * @param player 玩家类型(1或者-1)
	 */
	void play(const int& col, const int& player);
	/**
	 * 悔棋
	 * @param col 放在第几列(0-WIDTH-1)
	 */
	void undo(const int& col);
	void initPlay(const string& s);
	/**
	 * 下完这一步能不能赢
	 * @param col 放在第几列(0-WIDTH-1)
	 * @param predict 为true则还没有执行play，false则已经执行了play
	 * @param 是否是在判断对手的
	 * @return 是否胜利
	 */
	bool isWinMove(const int& lastCol, const bool& predict, const bool& isOpponent = false)const;
	/**
	 * 是否胜利或者和棋
	 * @param lastCol下的最后一列
	 * @param player 1代表X,-1代表O
	 * @return 是否胜利或者和棋
	 */
	bool isEnd(const int& lastCol, const int& player)const;
	/**
	 * 人人对战
	 */
	void pvp();
	/**
	 * 人机对战
	 * @param firstFlag 人类是否先手
	 */
	void pve(const bool& firstFlag);
	/**
	 * AI课的socket
	 */
	void AI_course_socket();
private:
	int ttHitCnt = 0;
	int searchedPosition = 0;
	WSADATA wsaData;
	//客户端Socket
	SOCKET sockClient;
	//服务端地址
	SOCKADDR_IN addrServer;
	static const int Len = 1024;

	//成功连接
	const string Success = "success";
	//游戏开始
	const string Begin = "begin";
	//游戏结束
	const string GameOver = "game_over";

};