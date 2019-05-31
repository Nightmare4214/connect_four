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
	//��ǰ��״̬
	ConnectFourState currentState;
	void update(const int& reward);
	/**
	 * ɾ�����˸��ڵ���������нڵ�,����ʱ����this
	 * @param t Ҫɾ�����ӵĽڵ�
	 */
	void destroy(MCTSNode *t);
	/**
	 * ���ɺ���
	 */
	void generateChildren();
	/**
	 * �Ƿ����еĺ��Ӷ���չ��
	 * @return �Ƿ�ȫ����չ��
	 */
	bool isAllExpanded();
	/**
	 * ѡ��һ�����ӽ�����չ
	 * @return ��չ�ڵ�
	 */
	MCTSNode* expand();
	/**
	 * �����õĺ���
	 * @param C ϵ��
	 * @return ʹ��UCB�㷨��Ȩ��exploration��exploitation��ѡ��÷���ߵ��ӽڵ㣬ע�������Ԥ��׶�ֱ��ѡ��ǰQֵ�÷���ߵġ�
	 */
	MCTSNode* getBestChild(const double& C = 0);
};