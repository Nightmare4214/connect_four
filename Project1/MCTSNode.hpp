#pragma once
#include"ConnectFourState.hpp"
#include<random>
class MCTSNode {
private:
	random_device rd;
public:
	MCTSNode();
	MCTSNode(const ConnectFourState& state);
	MCTSNode(const ConnectFourState& preState, const int& col);
	void init(const ConnectFourState& state);
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
	//�Ƿ����ɹ�����
	bool expanded;
	ConnectFourState currentState;
	void update(const int& reward);
	//ɾ�����˸��ڵ���������нڵ�,����this
	void destory(MCTSNode* t);
	//���ɺ���
	void generateChildren();
	//�Ƿ�ȫ����չ��
	bool isAllExpanded();
	//�����չ�ڵ�
	MCTSNode* expand();
	//�����õĺ���
	MCTSNode* getBestChild(const double& C = 0);
};