#include "ConnectFourState.hpp"

ConnectFourState::ConnectFourState(const uint64_t & board,
	const uint64_t & mask,
	const int & round) :board(board), mask(mask), round(round),lastMove(-1)
{
}

ConnectFourState::ConnectFourState(const int & col,
	const uint64_t & board,
	const uint64_t & mask,
	const int & round) : board(board), mask(mask), round(round)
{
	play(col);
}

int ConnectFourState::getPlayer() const
{
	if (0 == (round & 1))return 1;
	return -1;
}

uint64_t ConnectFourState::topMask(const int & col)
{
	/*
	1ULL << (HEIGHT - 1)��1�Ƶ�HEIGHT-1��λ��(Ҳ�������̵����)
	Ȼ���Ƶ���col��
	*/
	return (1ULL << (HEIGHT - 1)) << ((HEIGHT + 1)*col);
}

bool ConnectFourState::canPlay(const int & col) const
{
	return 0 == (mask&topMask(col));
}

uint64_t ConnectFourState::bottomMask(const int & col)
{
	return 1ULL << ((HEIGHT + 1)*col);
}

void ConnectFourState::play(const int & col)
{
	lastMove = col;
	//boardת���ɶ��ֵ�����
	board ^= mask;
	//mask��col�м�һ����
	mask |= mask + bottomMask(col);
	++round;
}

bool ConnectFourState::isWinMove(const int & col, const bool & opponentFlag)
{
	uint64_t tempBoard = board;
	if (opponentFlag) {
		tempBoard ^= mask;
	}
	/*
	��ֱ
	tempBorad   tempMask   tempMask&(tempMask >> 2
	1         0 0        0 0
	1         1 1        0 0
	1         1 1        0 0
	1         1 1        1 1
	0         1 0        1 0
	0         0 0        1 0
	*/
	uint64_t tempMask = tempBoard & (tempBoard >> 1);
	if (0 != (tempMask&(tempMask >> 2)))return true;

	//ˮƽ
	//����height+1�൱���Ƶ�ǰһ��ͬһ��
	tempMask = tempBoard & (tempBoard >> (HEIGHT + 1));
	if (0 != (tempMask&(tempMask >> ((HEIGHT + 1) << 1))))return true;

	//���Խ���
	//����height+2�൱���Ƶ�ǰһ��ͬһ�е���һ��
	tempMask = tempBoard & (tempBoard >> (HEIGHT + 2));
	if (0 != (tempMask&(tempMask >> ((HEIGHT + 2) << 1))))return true;

	//���Խ���
	//����height+1�൱���Ƶ�ǰһ��ͬһ�е���һ��
	tempMask = tempBoard & (tempBoard >> HEIGHT);
	return 0 != (tempMask&(tempMask >> (HEIGHT << 1)));
}

void ConnectFourState::printBoard() const
{
	char player = 'X', opponent = 'O';
	if (0 == (round & 1))swap(player, opponent);
	for (int i = HEIGHT - 1; i >= 0; --i) {
		uint64_t temp = 1ULL << i;
		for (int j = 0; j < WIDTH; ++j) {
			cout << '|';
			uint64_t tempMask = mask & temp;
			uint64_t tempBoard = board & temp;
			if (0 == tempMask)cout << '_';
			else if (0 != (tempMask^tempBoard))cout << player;
			else cout << opponent;
			temp <<= HEIGHT + 1;
		}
		cout << '|' << endl;
	}
	for (int i = 0; i < WIDTH; ++i) {
		cout << ' ' << i;
	}
	cout << ' ' << endl;
}

int ConnectFourState::getResult()
{
	bool xFlag = true;
	if (0 == (round & 1))xFlag = false;
	if (lastMove < 0)return -2;
	else if (isWinMove(lastMove, xFlag))return 1;
	else if (isWinMove(lastMove, !xFlag))return -1;
	else if (HEIGHT*WIDTH == round)return 0;
	else return -2;
}

bool ConnectFourState::isTerminal()
{
	int result = getResult();
	if (-2 == result)return false;
#ifdef _DEBUG
	if (1 == result)cout << "X�����ʤ��";
	else if (-1 == result)cout << "O�����ʤ��";
	else cout << "ƽ��";
	cout << endl;
#endif // DEBUG
	return true;
}
