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

static constexpr uint64_t getAllBottomMask(const int& WIDTH, const int& HEIGHT) {
	uint64_t result = 1, temp = 1;
	for (int i = 1; i < WIDTH; ++i) {
		temp <<= (HEIGHT + 1);
		result |= temp;
	}
	return result;
}

class Entry {
public:
	int score;
	int move;
	Entry(const int &move = 0, const int &score = 0) :score(score), move(move) {}
	void setData(const int &move, const int &score) {
		this->move = move;
		this->score = score;
	}
};

class AlphaBetaPruning {
	//�ߺͿ�
	static const int HEIGHT = 6, WIDTH = 8;
	//������
	static const int maxDepth = 14;
	//Ŀǰ�����
	int currentDepth;
	//�����ip
	const char *IP = "127.0.0.1";
	//����˵Ķ˿�
	const int PORT = 6000;
	//����
	const string TeamName = "Nightmare_alpha_beta_pruning_v2";

	TranspositionTable table;

	Entry moveSorter[WIDTH];
	int moveSorterSize;

	//�����һ��
	void addMove(const int& move, const int& score);

	//�����һ��
	int getNextMove();

	//�������ȥ�ķ���
	int getScore(const uint64_t& currentMove)const;

	//�����
	static const int INF = 0x3f3f3f3f;
	//����,0�����_��1�����(����)X��-1�����O
	int board[WIDTH][HEIGHT];
	//��ǰÿ���еĸ߶�
	int currentHeight[WIDTH];
	//�ڼ��غ�
	int round;
	//����˳��
	int columnOrder[WIDTH];
	/*λ���̣�ֻ�浱ǰ��ҵ�����
	HEIGHT+1λ��1����(��ߵ�������ʾ����)
	ͨ��^�������ı䵱ǰ��ҵ�����
	*/
	uint64_t bitBoard;
	/*
	����,�����е����ӣ���1��ʾ
	ͨ�����е������+1�������������
	*/
	uint64_t mask;

	//�ƶ���col����ߵ�λ��
	static uint64_t topMask(const int& col);

	//�ƶ���col�������
	static uint64_t bottomMask(const int& col);

	//ȡ����
	static uint64_t columnMask(const int & col);

	//����ȫ1
	static const uint64_t allBottomMask = getAllBottomMask(WIDTH, HEIGHT);
	//�������еĺϷ�λ��
	static const uint64_t boardValidMask = allBottomMask * ((1ULL << HEIGHT) - 1);

	//���position����������Ӯ��λ��(�������յ�)
	static uint64_t getWinPositions(const uint64_t& position, const uint64_t &mask);

	//���������һ�����µĵط�
	uint64_t getAllValidPositions()const;

	//��õ�ǰ���������Ӯ�ĺϷ�λ��
	uint64_t getAllCurrentPlayerWinPositions()const;

	//��õ�ǰ���������Ӯ�ĺϷ�λ��
	uint64_t getAllOpponentWinPositions()const;

	/**
	 * �����һ���������λ��
	 * @param forcedMove ���������������ʤ�㣬���¼�����������
	 * @return ��һ���������λ��
	 */
	uint64_t getAllNotLosePositions(int& forcedMove);

	uint64_t getKey()const;

	/**
	 * �������������ȷ����
	 * @return ��ȷ����
	 */
	int getCorrectCol()const;
	/**
	 * ��������÷֣�����Ϊ���еĿ�λ�÷������Ӻ��м���Ӯ��
	 * @param player 1����X,-1����O
	 * @param opponentFlag �Ƿ��Ƕ���
	 * @return �÷�
	 */
	int evaluate(const int& player, const bool& opponentFlag = false)const;
	/**
	 * alpha-beta��֦
	 * @param depth �������
	 * @param alpha
	 * @param beta
	 * @param player ��ң�1����X,-1����O
	 * @param move �µ���
	 * @return alphaֵ
	 */
	int alphaBetaPruning(int depth, int alpha, int beta, int player, int& move);

	int getColByAlphaBetaPruning(const int& player);
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
	AlphaBetaPruning();
	/**
	 * �������
	 */
	void printBoard()const;
	/**
	 * ��һ���ܲ��ܷ�����
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 * @return ��һ���ܲ��ܷ�����
	 */
	bool canPlay(const int& col)const;
	/**
	 * ������
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 * @param player �������(1����-1)
	 */
	void play(const int& col, const int& player);
	/**
	 * ����
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 */
	void undo(const int& col);
	void initPlay(const string& s);
	/**
	 * ������һ���ܲ���Ӯ
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 * @param predict Ϊtrue��û��ִ��play��false���Ѿ�ִ����play
	 * @param �Ƿ������ж϶��ֵ�
	 * @return �Ƿ�ʤ��
	 */
	bool isWinMove(const int& lastCol, const bool& predict, const bool& isOpponent = false)const;
	/**
	 * �Ƿ�ʤ�����ߺ���
	 * @param lastCol�µ����һ��
	 * @param player 1����X,-1����O
	 * @return �Ƿ�ʤ�����ߺ���
	 */
	bool isEnd(const int& lastCol, const int& player)const;
	/**
	 * ���˶�ս
	 */
	void pvp();
	/**
	 * �˻���ս
	 * @param firstFlag �����Ƿ�����
	 */
	void pve(const bool& firstFlag);
	/**
	 * AI�ε�socket
	 */
	void AI_course_socket();
private:
	int ttHitCnt = 0;
	int searchedPosition = 0;
	WSADATA wsaData;
	//�ͻ���Socket
	SOCKET sockClient;
	//����˵�ַ
	SOCKADDR_IN addrServer;
	static const int Len = 1024;

	//�ɹ�����
	const string Success = "success";
	//��Ϸ��ʼ
	const string Begin = "begin";
	//��Ϸ����
	const string GameOver = "game_over";

};