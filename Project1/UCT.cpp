#include "UCT.hpp"



UCT::UCT()
{
}

UCT::UCT(const ConnectFourState& t)
{
	root.init(t);
}

void UCT::init(const ConnectFourState & t)
{
	root.init(t);
}

MCTSNode* UCT::treePolicy()
{
	MCTSNode* node = &root;
	while (!(node->currentState.isTerminal())) {
		if (node->isAllExpanded()) {
			node = node->getBestChild(C);
		}
		else {
			return node->expand();
		}
	}
	return node;
}

int UCT::defaultPolicy(ConnectFourState t)
{
	int playerMoves[ConnectFourState::WIDTH];
	int opponentWinMoves[2];
	int winMove = -1;
	int playerSize = 0;
	int opponentSize = 0;
	while (!t.isTerminal()) {
		winMove = -1;
		playerSize = 0;
		opponentSize = 0;
		//遍历选择
		for (int i = 0; i < ConnectFourState::WIDTH; ++i) {
			//当前选手的制胜点
			if (t.canPlay(i)) {
				if (t.isWinMove(i, false, true)) {
					t.play(i);
					return t.getResult();
				}
				else {
					playerMoves[playerSize] = i;
					++playerSize;
				}
				//对手的制胜点
				if (opponentSize < 2) {
					if (t.isWinMove(i, true, true)) {
						opponentWinMoves[opponentSize] = i;
						++opponentSize;
					}
				}
			}
		}
		if (2 == opponentSize) {
			t.play(opponentWinMoves[0]);
			t.play(opponentWinMoves[1]);
			return t.getResult();
		}
		else if (1 == opponentSize) {
			t.play(opponentWinMoves[0]);
		}
		else {
			t.play(playerMoves[rd() % playerSize]);
		}
	}
	return t.getResult();
}

void UCT::backUp(MCTSNode * expandNode,int reward)
{
	if (-1 == root.currentState.getPlayer())reward = -reward;
	if (reward < 0)reward = 0;
	while (nullptr != expandNode) {
		expandNode->update(reward);
		expandNode = expandNode->parent;
	}
}

int UCT::getColByUCT()
{
	int playerMoves[ConnectFourState::WIDTH];
	int opponentWinMoves[2];
	int winMove = -1;
	int playerSize = 0;
	int opponentSize = 0;
	ConnectFourState t = root.currentState;
	for (int i = 0; i < ConnectFourState::WIDTH; ++i) {
		if (t.canPlay(i)) {
			//当前选手制胜
			if (t.isWinMove(i, false, true)) {
				return i;
			}
			else {
				playerMoves[playerSize] = i;
				++playerSize;
			}
			//对手的制胜点
			if (opponentSize < 2) {
				if (t.isWinMove(i, true, true)) {
					opponentWinMoves[opponentSize] = i;
					++opponentSize;
				}
			}
		}
	}
	if (opponentSize>0) {
		return opponentWinMoves[0];
	}
	MCTSNode* expandNode = nullptr;
	for (int epoch = 0; epoch < 50000; ++epoch) {
		expandNode = treePolicy();
		int reward = defaultPolicy(expandNode->currentState);
		backUp(expandNode, reward);
	}
	expandNode = root.getBestChild(0);
	return expandNode->currentState.lastMove;
}
