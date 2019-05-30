#pragma once
#include"ConnectFourState.hpp"
#include"MCTSNode.hpp"
#include"UCT.hpp"
class ConnectFour {
private:
	int getCorrectCol();
public:
	ConnectFourState board;
	UCT solver;
	//玩家对战玩家
	void pvp();
	void pve(const bool& playerFirst);
};