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
	SOCKET sockClient;//�ͻ���Socket
	SOCKADDR_IN addrServer;//����˵�ַ
	static const int Len = 1024;
	const char *IP = "127.0.0.1";
	const int PORT = 6000;

	const string Success = "success";
	const string Begin = "begin";
	const string GameOver = "game_over";

	const string TeamName = "faQ";
	//��ʼ��socket
	void initSocket();
	//�ر�socket
	void closeSocket();
	//������Ϣ
	void sendData(const string& msg)const;
	//������Ϣ
	string recvData()const;
public:
	ConnectFourState board;
	UCT solver;
	//��Ҷ�ս���
	void pvp();
	void pve(const bool& playerFirst);
	//AI�ε�socket
	void AI_course_socket();
};