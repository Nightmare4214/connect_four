#include "MCTSNode.hpp"
#include<limits>
#include<cmath>
MCTSNode::MCTSNode():
	parent(nullptr),
	childrenSize(0),
	remainChildrenSize(0),
	qualityValue(0),
	visitTime(0),
	expanded(false)
{
}

MCTSNode::MCTSNode(const ConnectFourState & state) :
	parent(nullptr), 
	childrenSize(0), 
	remainChildrenSize(0), 
	qualityValue(0), 
	visitTime(0), 
	expanded(false),
	currentState(state)
{
}

MCTSNode::MCTSNode(const ConnectFourState & preState, const int & col):
	parent(nullptr),
	childrenSize(0),
	remainChildrenSize(0),
	qualityValue(0),
	visitTime(0),
	expanded(false),
	currentState(preState)
{
	currentState.play(col);
}

void MCTSNode::init(const ConnectFourState& state)
{
	destory(this);
	parent = nullptr;
	childrenSize = 0;
	remainChildrenSize = 0;
	qualityValue = 0;
	visitTime = 0;
	expanded = false;
	currentState = state;
}

void MCTSNode::update(const int & reward)
{
	qualityValue += reward;
	++visitTime;
}

void MCTSNode::destory(MCTSNode * t)
{
	for (int i = 0; i < childrenSize; ++i) {
		if (nullptr != t->children[i]) {
			destory(t->children[i]);
			delete t->children[i];
			t->children[i] = nullptr;
		}
	}
}

void MCTSNode::generateChildren()
{
	for (int i = 0; i < ConnectFourState::WIDTH; ++i) {
		if (currentState.canPlay(i)) {
			children[childrenSize] = new MCTSNode(currentState, i);
			children[childrenSize]->parent = this;
			++childrenSize;
			++remainChildrenSize;
		}
	}
	expanded = true;
}

bool MCTSNode::isAllExpanded()
{
	if (!expanded)generateChildren();
	return 0 == remainChildrenSize;
}

MCTSNode * MCTSNode::expand()
{
	
	int id = rd() % remainChildrenSize;
	--remainChildrenSize;
	swap(children[id], children[remainChildrenSize]);
	return children[remainChildrenSize];
}

MCTSNode * MCTSNode::getBestChild(const double& C)
{
	int id[ConnectFourState::WIDTH];
	int size = 0;
	double maxUCB = -DBL_MIN;
	for (int i = 0; i < childrenSize; ++i) {
		MCTSNode* tempChild = children[i];
		double tempUCB = tempChild->qualityValue*1.0 / tempChild->visitTime + C * sqrt(2 * log(visitTime*1.0 / tempChild->visitTime));
		if (tempUCB > maxUCB) {
			id[0] = i;
			size = 1;
			maxUCB = tempUCB;
		}
		else if (fabs(maxUCB - tempUCB) < 1e-6) {
			id[size] = i;
			++size;
		}
	}
	return children[rd() % size];
}
