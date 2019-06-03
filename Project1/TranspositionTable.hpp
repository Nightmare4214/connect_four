#pragma once
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
//�û���
class TranspositionTableNode {
public:
	//��ȷֵ
	static const int EXACT_FLAG = 0;
	//<=׼ȷֵ
	static const int ALPHA_FLAG = 1;
	//>=׼ȷֵ
	static const int BETA_FLAG = 2;
	//hashֵ
	uint64_t key;
	//����ֵ
	int value;
	//���
	int depth;
	//����ֵflag
	int flag;
	//��õĲ���
	int bestMove;
	TranspositionTableNode() :key(0), value(0), depth(0), flag(0), bestMove(-1) {}
	void setValue(const uint64_t& key, const int& value, const int& depth, const int& flag, const int& bestMove);
};
class TranspositionTable {
public:
	const int tableSize = 1 << 23;
	uint64_t tableSizeMask = tableSize - 1;
	vector<TranspositionTableNode> table;
	static const int unknown = 0x80000000;
	TranspositionTable();
	void reset();
	/**
	 * ̽���û�������û�е�ǰ����
	 * @param depth ������depth��õ��ĵ÷�
	 * @param alpha ��ǰalpha
	 * @param beta ��ǰbeta
	 * @param bestMove ����������õĲ���
	 * @param findedFlag �����Ƿ����û������ҵ�
	 * @return �ҵ��򷵻ؾ���÷֣��Ҳ�������unknown
	 */
	int probeHash(const uint64_t & key, const int & depth, const int & alpha, const int & beta, int& bestMove, bool& findedFlag);
	/**
	 * �û����м�¼����
	 * @param value ����÷�
	 * @param depth ������depth��õ��ĵ÷�
	 * @param flag ����ֵ����
	 */
	void recordHash(const uint64_t& key, const int& value, const int& depth, const int& flag, const int& bestMove);
};