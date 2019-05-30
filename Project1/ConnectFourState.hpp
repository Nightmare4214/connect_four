#pragma once
#include<iostream>
using namespace std;
class ConnectFourState {
private:
	//�ƶ���col����ߵ�λ��
	static uint64_t topMask(const int & col);
	//�ƶ���col�������
	static uint64_t bottomMask(const int & col);
	//ȡ����
	static uint64_t columnMask(const int & col);
	//�Լ�������
	uint64_t board;
	//���е�����
	uint64_t mask;
	
public:
	//���һ��
	int lastMove;
	static const int WIDTH = 8;
	static const int HEIGHT = 6;
	//�غ���
	int round;
	ConnectFourState(const uint64_t& board = 0,const uint64_t& mask = 0, const int& round = 0);
	ConnectFourState(const int& col, const uint64_t& board = 0, const uint64_t& mask = 0, const int& round = 0);
	//��õ�ǰ�����
	int getPlayer() const;
	//��һ���Ƿ�����
	bool canPlay(const int &col)const;
	/**
	 * ������
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 */
	void play(const int &col);
	void init(const string &s) {
		for (const char &c : s)play(c - '0');
	}
	/**
	 * �ж���һ������ȥ�ܲ���Ӯ
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 * @param opponentFlag �Ƕ���
	 * @param predict ��һ����û��
	 * @return  �ж���һ������ȥ�ܲ���Ӯ
	 */
	bool isWinMove(const int &col, const bool &opponentFlag, const bool& predict = false)const;
	//�������
	void printBoard()const;
	/**
	 * �����Ϸ���
	 * @return col Xʤ����Ϊ1��ƽ��Ϊ0��Oʤ����Ϊ-1,��Ϸû��������-2
	 */
	int getResult()const;
	/**
	 * �ж���Ϸ�Ƿ����
	 * @return �ж���Ϸ�Ƿ����
	 */
	bool isTerminal()const;
};