#pragma once
#include"ConnectFourState.hpp"
class MCTSNode {
public:
	MCTSNode();
	//父亲节点
	MCTSNode* parent;
	//孩子节点，因为只有WIDTH种选择，所以长度是WIDTH
	MCTSNode* children[ConnectFourState::WIDTH];
	int childrenSize;
	//没访问过的孩子个数
	int remainChildrenSize;
	//胜利次数
	int qualityValue;
	//访问次数
	int visitTime;
	ConnectFourState currentState;
	void update(const int& reward);
	//删除除了根节点以外的所有节点,传入this
	void destory(MCTSNode* t) {
		for (int i = 0; i < childrenSize; ++i) {
			if (nullptr != children[i])destory(children[i]);
			delete children[i];
			children[i] = nullptr;
		}
	}
};