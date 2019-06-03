#pragma once
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
//置换表
class TranspositionTableNode {
public:
	//精确值
	static const int EXACT_FLAG = 0;
	//<=准确值
	static const int ALPHA_FLAG = 1;
	//>=准确值
	static const int BETA_FLAG = 2;
	//hash值
	uint64_t key;
	//评估值
	int value;
	//深度
	int depth;
	//评估值flag
	int flag;
	//最好的步骤
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
	 * 探测置换表中有没有当前局面
	 * @param depth 搜索了depth层得到的得分
	 * @param alpha 当前alpha
	 * @param beta 当前beta
	 * @param bestMove 用来接收最好的步骤
	 * @param findedFlag 接收是否在置换表中找到
	 * @return 找到则返回局面得分，找不到返回unknown
	 */
	int probeHash(const uint64_t & key, const int & depth, const int & alpha, const int & beta, int& bestMove, bool& findedFlag);
	/**
	 * 置换表中记录局面
	 * @param value 局面得分
	 * @param depth 搜索了depth层得到的得分
	 * @param flag 评分值类型
	 */
	void recordHash(const uint64_t& key, const int& value, const int& depth, const int& flag, const int& bestMove);
};