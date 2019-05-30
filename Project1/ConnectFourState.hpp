#pragma once
#include<iostream>
using namespace std;
class ConnectFourState {
private:
	//�ƶ���col����ߵ�λ��
	static uint64_t topMask(const int &col);
	//�ƶ���col�������
	static uint64_t bottomMask(const int &col);
	//�Լ�������
	uint64_t board;
	//���е�����
	uint64_t mask;
	//���һ��
	int lastMove;
public:
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
	/**
	 * �ж���һ������ȥ�ܲ���Ӯ
	 * @param col ���ڵڼ���(0-WIDTH-1)
	 * @param opponentFlag �Ƕ���
	 * @return  �ж���һ������ȥ�ܲ���Ӯ
	 */
	bool isWinMove(const int &col,const bool &opponentFlag);
	//�������
	void printBoard()const;
	/**
	 * �����Ϸ���
	 * @return col Xʤ����Ϊ1��ƽ��Ϊ0��Oʤ����Ϊ-1,��Ϸû��������-2
	 */
	int getResult();
	/**
	 * �ж���Ϸ�Ƿ����
	 * @return �ж���Ϸ�Ƿ����
	 */
	bool isTerminal();
};