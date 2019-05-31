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
	//��û������������
	int getCorrectCol()const;
	WSADATA wsaData;
	//�ͻ���Socket
	SOCKET sockClient;
	//����˵�ַ
	SOCKADDR_IN addrServer;
	static const int Len = 1024;
	//�����ip
	const char *IP = "127.0.0.1";
	//����˵Ķ˿�
	const int PORT = 6000;
	//�ɹ�����
	const string Success = "success";
	//��Ϸ��ʼ
	const string Begin = "begin";
	//��Ϸ����
	const string GameOver = "game_over";
	//����
	const string TeamName = "faQ";
	/**
	 * ��ʼ��socket
	 */
	void initSocket();
	/**
	 * �ر�socket
	 */
	void closeSocket();
	/**
	 * ������Ϣ
	 * @param msg ���͵���Ϣ
	 */
	void sendData(const string& msg)const;
	/**
	 * ������Ϣ
	 * @return ���յ�����Ϣ
	 */
	string recvData()const;
public:
	//��ǰ������
	ConnectFourState board;
	//UCT�㷨
	UCT solver;
	/**
	 * ��Ҷ�ս���
	 */
	void pvp();
	/**
	 * ���vs AI
	 * @param playerFirst ����Ƿ�����
	 */
	void pve(const bool& playerFirst);
	/**
	 * AI�ε�socket
	 */
	void AI_course_socket();
};