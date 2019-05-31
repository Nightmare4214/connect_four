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
	/**
	 * 用游戏状态初始化
	 * @param t 游戏状态
	 */
	void init(const ConnectFourState& t);
	/**
	 * 蒙特卡罗树搜索的Selection和Expansion阶段，
	 * 根据exploration/exploitation算法返回最好的需要expend的节点，注意如果节点是叶子结点直接返回。
	 * 基本策略是:先找当前未选择过的子节点，如果有多个则随机选。
	 * 如果都选择过就找权衡过exploration/exploitation的UCB值最大的，如果UCB值相等则随机选。
	 * @return 需要expend的节点
	 */
	MCTSNode* treePolicy();
	/**
	 * 蒙特卡罗树搜索的Simulation阶段，输入一个游戏状态，随机操作后创建新的节点，返回新增节点的reward。
	 * 注意输入的节点应该不是子节点，而且是有未执行的Action可以expend的。
	 * 基本策略是随机选择Action。
	 * @param t 新增节点的游戏状态
	 * @return 新增节点的reward
	 */
	int defaultPolicy(ConnectFourState t);
	/**
	 * 蒙特卡洛树搜索的Back_propagation阶段:
	 * 输入前面获取需要expend的节点和新执行Action的reward，反馈给expend节点和上游所有节点并更新对应数据。
	 * @param expanded 需要expend的节点
	 * @param reward 反馈
	 */
	void backUp(MCTSNode* expanded, int reward);
	/**
	 * 通过UCT算法，获得下一步
	 * @return 一列
	 */
	int getColByUCT();
};