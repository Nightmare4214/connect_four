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
	//当前的状态
	ConnectFourState currentState;
	void update(const int& reward);
	/**
	 * 删除除了根节点以外的所有节点,调用时传入this
	 * @param t 要删除孩子的节点
	 */
	void destroy(MCTSNode *t);
	/**
	 * 生成孩子
	 */
	void generateChildren();
	/**
	 * 是否所有的孩子都扩展过
	 * @return 是否全部扩展过
	 */
	bool isAllExpanded();
	/**
	 * 选择一个孩子进行扩展
	 * @return 扩展节点
	 */
	MCTSNode* expand();
	/**
	 * 获得最好的孩子
	 * @param C 系数
	 * @return 使用UCB算法，权衡exploration和exploitation后选择得分最高的子节点，注意如果是预测阶段直接选择当前Q值得分最高的。
	 */
	MCTSNode* getBestChild(const double& C = 0);
};