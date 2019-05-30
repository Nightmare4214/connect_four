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
	//是否生成过孩子
	bool expanded;
	ConnectFourState currentState;
	void update(const int& reward);
	//删除除了根节点以外的所有节点,传入this
	void destory(MCTSNode* t);
	//生成孩子
	void generateChildren();
	//是否全部扩展过
	bool isAllExpanded();
	//获得扩展节点
	MCTSNode* expand();
	//获得最好的孩子
	MCTSNode* getBestChild(const double& C = 0);
};