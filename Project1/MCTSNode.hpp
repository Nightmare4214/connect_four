#pragma once
#include"ConnectFourState.hpp"
class MCTSNode {
public:
	MCTSNode();
	//���׽ڵ�
	MCTSNode* parent;
	//���ӽڵ㣬��Ϊֻ��WIDTH��ѡ�����Գ�����WIDTH
	MCTSNode* children[ConnectFourState::WIDTH];
	int childrenSize;
	//û���ʹ��ĺ��Ӹ���
	int remainChildrenSize;
	//ʤ������
	int qualityValue;
	//���ʴ���
	int visitTime;
	ConnectFourState currentState;
	void update(const int& reward);
	//ɾ�����˸��ڵ���������нڵ�,����this
	void destory(MCTSNode* t) {
		for (int i = 0; i < childrenSize; ++i) {
			if (nullptr != children[i])destory(children[i]);
			delete children[i];
			children[i] = nullptr;
		}
	}
};