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
	//��Ҷ�ս���
	void pvp();
	void pve(const bool& playerFirst);
};