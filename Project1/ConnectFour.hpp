#pragma once
#include"ConnectFourState.hpp"
#include"MCTSNode.hpp"
class ConnectFour {
private:
	int getCorrectCol();
public:
	ConnectFourState board;
	//玩家对战玩家
	void pvp();
};