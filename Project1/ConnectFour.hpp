#pragma once
#include"ConnectFourState.hpp"
#include"MCTSNode.hpp"
class ConnectFour {
private:
	int getCorrectCol();
public:
	ConnectFourState board;
	//��Ҷ�ս���
	void pvp();
};