#include "TranspositionTable.hpp"

void TranspositionTableNode::setValue(const uint64_t & key, const int & value, const int & depth, const int & flag, const int& bestMove)
{
	this->key = key;
	this->value = value;
	this->depth = depth;
	this->flag = flag;
	this->bestMove = bestMove;
}

TranspositionTable::TranspositionTable() :table(tableSize)
{

}

void TranspositionTable::reset()
{
	memset(&table[0], 0, tableSize * sizeof(TranspositionTableNode));
}

int TranspositionTable::probeHash(const uint64_t & key, const int & depth, const int & alpha, const int & beta, int& bestMove, bool& findedFlag)
{
	findedFlag = false;
	//以2为底的取余可以被与代替
	TranspositionTableNode& temp = table[key&tableSizeMask];
	//命中
	if (temp.key == key) {
		findedFlag = true;
		//只选择深度更深的
		if (temp.depth >= depth) {
			if (TranspositionTableNode::EXACT_FLAG == temp.flag) {
				bestMove = temp.bestMove;
				return temp.value;
			}
			else if (TranspositionTableNode::ALPHA_FLAG == temp.flag&&temp.value <= alpha) {
				return alpha;
			}
			else if (TranspositionTableNode::BETA_FLAG == temp.flag&&temp.value >= beta) {
				return beta;
			}
		}

	}
	return unknown;
}

void TranspositionTable::recordHash(const uint64_t & key, const int & value, const int & depth, const int & flag, const int& bestMove)
{
	TranspositionTableNode& temp = table[tableSizeMask&key];
	if (0 != temp.key&&temp.depth > depth)return;
	temp.setValue(key, value, depth, flag, bestMove);
}
