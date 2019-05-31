#pragma once
#include"ConnectFourState.hpp"
#include"MCTSNode.hpp"
#include"UCT.hpp"
#pragma comment(lib,"ws2_32.lib")
#include<Winsock2.h>
#include<Ws2tcpip.h>
#include<cstdio>
#include<iostream>
#include<random>
#include<string>
class ConnectFour {
private:
	//获得获得玩家输入的列
	int getCorrectCol()const;
	WSADATA wsaData;
	//客户端Socket
	SOCKET sockClient;
	//服务端地址
	SOCKADDR_IN addrServer;
	static const int Len = 1024;
	//服务端ip
	const char *IP = "127.0.0.1";
	//服务端的端口
	const int PORT = 6000;
	//成功连接
	const string Success = "success";
	//游戏开始
	const string Begin = "begin";
	//游戏结束
	const string GameOver = "game_over";
	//队名
	const string TeamName = "faQ";
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
	//当前的棋盘
	ConnectFourState board;
	//UCT算法
	UCT solver;
	/**
	 * 玩家对战玩家
	 */
	void pvp();
	/**
	 * 玩家vs AI
	 * @param playerFirst 玩家是否先手
	 */
	void pve(const bool& playerFirst);
	/**
	 * AI课的socket
	 */
	void AI_course_socket();
};