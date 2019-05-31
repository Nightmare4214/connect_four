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
	int getCorrectCol();
	WSADATA wsaData;
	SOCKET sockClient;//客户端Socket
	SOCKADDR_IN addrServer;//服务端地址
	static const int Len = 1024;
	const char *IP = "127.0.0.1";
	const int PORT = 6000;

	const string Success = "success";
	const string Begin = "begin";
	const string GameOver = "game_over";

	const string TeamName = "faQ";
	//初始化socket
	void initSocket();
	//关闭socket
	void closeSocket();
	//发送消息
	void sendData(const string& msg)const;
	//接收消息
	string recvData()const;
public:
	ConnectFourState board;
	UCT solver;
	//玩家对战玩家
	void pvp();
	void pve(const bool& playerFirst);
	//AI课的socket
	void AI_course_socket();
};