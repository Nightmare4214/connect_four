#include "MCTSNode.hpp"

MCTSNode::MCTSNode()
{
}

void MCTSNode::update(const int & reward)
{
	qualityValue += reward;
	++visitTime;
}
