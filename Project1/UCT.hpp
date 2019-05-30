#pragma once
#include"ConnectFourState.hpp"
#include"MCTSNode.hpp"
#include<cmath>
class UCT {
private:
	random_device rd;
public:
	const double C = 1 / sqrt(2);
	MCTSNode root;
	UCT();
	UCT(const ConnectFourState& t);
	void init(const ConnectFourState& t);
	MCTSNode* treePolicy();
	int defaultPolicy(ConnectFourState t);
	void backUp(MCTSNode* expanded,int reward);
	int getColByUCT();
};