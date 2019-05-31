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
	 * ����Ϸ״̬��ʼ��
	 * @param t ��Ϸ״̬
	 */
	void init(const ConnectFourState& t);
	/**
	 * ���ؿ�����������Selection��Expansion�׶Σ�
	 * ����exploration/exploitation�㷨������õ���Ҫexpend�Ľڵ㣬ע������ڵ���Ҷ�ӽ��ֱ�ӷ��ء�
	 * ����������:���ҵ�ǰδѡ������ӽڵ㣬����ж�������ѡ��
	 * �����ѡ�������Ȩ���exploration/exploitation��UCBֵ���ģ����UCBֵ��������ѡ��
	 * @return ��Ҫexpend�Ľڵ�
	 */
	MCTSNode* treePolicy();
	/**
	 * ���ؿ�����������Simulation�׶Σ�����һ����Ϸ״̬����������󴴽��µĽڵ㣬���������ڵ��reward��
	 * ע������Ľڵ�Ӧ�ò����ӽڵ㣬��������δִ�е�Action����expend�ġ�
	 * �������������ѡ��Action��
	 * @param t �����ڵ����Ϸ״̬
	 * @return �����ڵ��reward
	 */
	int defaultPolicy(ConnectFourState t);
	/**
	 * ���ؿ�����������Back_propagation�׶�:
	 * ����ǰ���ȡ��Ҫexpend�Ľڵ����ִ��Action��reward��������expend�ڵ���������нڵ㲢���¶�Ӧ���ݡ�
	 * @param expanded ��Ҫexpend�Ľڵ�
	 * @param reward ����
	 */
	void backUp(MCTSNode* expanded, int reward);
	/**
	 * ͨ��UCT�㷨�������һ��
	 * @return һ��
	 */
	int getColByUCT();
};