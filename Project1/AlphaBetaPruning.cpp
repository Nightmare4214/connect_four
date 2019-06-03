#include "AlphaBetaPruning.hpp"
#include"TranspositionTable.hpp"
#include<conio.h>

int AlphaBetaPruning::getScore(const uint64_t & currentMove)const
{
	uint64_t pos = getWinPositions(bitBoard | currentMove, mask);
	int cnt = 0;
	while (0 != pos) {
		++cnt;
		pos &= pos - 1;
	}
	return cnt;
}

uint64_t AlphaBetaPruning::topMask(const int & col)
{
	/*
	1ULL << (HEIGHT - 1)将1移到HEIGHT-1的位置(也就是棋盘的最高)
	然后移到第col列
	*/
	return (1ULL << (HEIGHT - 1)) << ((HEIGHT + 1)*col);
}

uint64_t AlphaBetaPruning::bottomMask(const int & col)
{
	return 1ULL << ((HEIGHT + 1)*col);
}

uint64_t AlphaBetaPruning::columnMask(const int & col)
{
	return (((1ULL << HEIGHT) - 1) << (col*(HEIGHT + 1)));
}

uint64_t AlphaBetaPruning::getWinPositions(const uint64_t & position, const uint64_t & boardMask)
{
	//垂直的(已经形成连续3个)
	uint64_t result = (position << 1) & (position << 2) & (position << 3);

	//水平
	uint64_t temp = (position << (HEIGHT + 1))&(position << ((HEIGHT + 1) << 1));
	//形成3个，制胜点在右边的
	result |= temp & (position << (3 * (HEIGHT + 1)));
	//xx_x这种
	result |= temp & (position >> (HEIGHT + 1));
	temp = (position >> (HEIGHT + 1))&(position >> ((HEIGHT + 1) << 1));
	//形成3个，制胜点在左边的
	result |= temp & (position >> (3 * (HEIGHT + 1)));
	//x_xx这种
	result |= temp & (position << (HEIGHT + 1));

	//主对角线
	temp = (position << HEIGHT)&(position << (HEIGHT << 1));
	//形成3个，制胜点在右下方
	result |= temp & (position << (3 * HEIGHT));
	//斜着xx_x这种
	result |= temp & (position >> HEIGHT);
	temp = (position >> HEIGHT)&(position >> (HEIGHT << 1));
	//形成3个，制胜点在左上方
	result |= temp & (position >> (3 * HEIGHT));
	//斜着x_xx这种
	result |= temp & (position << HEIGHT);

	//副对角线
	temp = (position << (HEIGHT + 2))&(position << ((HEIGHT + 2) << 1));
	//形成3个，制胜点在右上方
	result |= temp & (position << (3 * (HEIGHT + 2)));
	//斜着xx_x这种
	result |= temp & (position >> (HEIGHT + 2));
	temp = (position >> (HEIGHT + 2))&(position >> ((HEIGHT + 2) << 1));
	//形成3个，制胜点在左下方
	result |= temp & (position >> (3 * (HEIGHT + 2)));
	//斜着x_xx这种
	result |= temp & (position << (HEIGHT + 2));

	return result & (boardValidMask ^ boardMask);
}

uint64_t AlphaBetaPruning::getAllValidPositions() const
{
	return (mask + allBottomMask)&boardValidMask;
}

uint64_t AlphaBetaPruning::getAllCurrentPlayerWinPositions() const
{
	return getWinPositions(bitBoard, mask);
}

uint64_t AlphaBetaPruning::getAllOpponentWinPositions() const
{
	return getWinPositions(bitBoard^mask, mask);
}

uint64_t AlphaBetaPruning::getAllNotLosePositions(int & forcedMove)
{
	//所有能下的位置
	uint64_t validMask = getAllValidPositions();
	//对手会赢的位置
	uint64_t opponentWinPositions = getAllOpponentWinPositions();
	//对手能赢的合法位置
	uint64_t forcedMask = validMask & opponentWinPositions;
	if (0 != forcedMask) {
		//forcedMask-1相当于破坏第一个制胜点，如果与之后还不为0，说明双迫手
		if (0 != (forcedMask&(forcedMask - 1))) {
			for (int i = 0; i < WIDTH; ++i) {
				//双迫手就随便找个地方下吧
				if (0 != (columnMask(i)&forcedMask)) {
					forcedMove = i;
					return 0;
				}
			}
			return 0;
		}//对手只有一个制胜点,不下这就跪了
		else return forcedMask;
	}
	//opponentWinPositions >> 1 将对手能赢的位置下移一位
	uint64_t nextPosition = validMask & (~(opponentWinPositions >> 1));
	if (0 == nextPosition) {
		for (int i = 0; i < WIDTH; ++i) {
			//下完对手就赢了，然而没别的选择了
			if (0 != (columnMask(i)&validMask)) {
				forcedMove = i;
				return 0;
			}
		}
		return 0;
	}
	else return nextPosition;
}

uint64_t AlphaBetaPruning::getKey() const
{
	return bitBoard + mask + allBottomMask;
}

int AlphaBetaPruning::getCorrectCol() const
{
	string temp;
	int col;
	while (cin >> temp) {
		try {
			col = stoi(temp);
			if (col < 0 || col >= WIDTH) {
				_cprintf("row ERROR,please enter the correct row\r\n");
			}
			else if (!canPlay(col)) {
				_cprintf("row full,please enter the correct row\r\n");
			}
			else return col;
		}
		catch (exception e) {
			_cprintf("row ERROR,please enter the correct row\r\n");
		}
	}
	return 0;
}

int AlphaBetaPruning::evaluate(const int & player, const bool& opponentFlag) const
{
	int opponent = -player;
	int score = 0;
	//竖着的
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j <= HEIGHT - 4; ++j) {
			int playerCnt = 0, opponentCnt = 0;
			for (int k = 0; k < 4; ++k) {
				int temp = board[i][j + k];
				if (temp == player)++playerCnt;
				else if (temp == opponent) {
					++opponentCnt;
					break;
				}
			}
			if (opponentCnt != 0)continue;
			else if (4 == playerCnt)score += 1000000;
			else if (3 == playerCnt)score += 100000;
			else if (2 == playerCnt)score += 100;
			else if (1 == playerCnt)score += 50;
		}
	}
	//横直的
	for (int j = 0; j < HEIGHT; ++j) {
		for (int i = 0; i <= WIDTH - 4; ++i) {
			int playerCnt = 0, opponentCnt = 0;
			for (int k = 0; k < 4; ++k) {
				int temp = board[i + k][j];
				if (temp == player)++playerCnt;
				else if (temp == opponent) {
					++opponentCnt;
					break;
				}
			}
			if (opponentCnt != 0)continue;
			else if (4 == playerCnt)score += 1000000;
			else if (3 == playerCnt)score += 50000;
			else if (2 == playerCnt)score += 100;
			else if (1 == playerCnt)score += 50;

		}
	}
	//主对角线y=-x+bias
	for (int bias = 3; bias <= HEIGHT + WIDTH - 5; ++bias) {
		int x = 0, y = bias;
		if (y >= HEIGHT) {
			y = HEIGHT - 1;
			x = -y + bias;
		}
		while (x < WIDTH&&y >= 0 && x + 3 < WIDTH&&y - 3 >= 0) {
			int playerCnt = 0, opponentCnt = 0;
			for (int k = 0; k < 4; ++k) {
				int temp = board[x + k][y - k];
				if (temp == player)++playerCnt;
				else if (temp == opponent) {
					++opponentCnt;
					break;
				}
			}
			++x;
			--y;
			if (opponentCnt != 0)continue;
			else if (4 == playerCnt)score += 1000000;
			else if (3 == playerCnt)score += 30000;
			else if (2 == playerCnt)score += 100;
			else if (1 == playerCnt)score += 50;
		}
	}
	//副对角线y=x+bias
	for (int bias = HEIGHT - 4; bias >= 4 - WIDTH; --bias) {
		int x = 0, y = bias;
		if (y < 0) {
			y = 0;
			x = -bias;
		}
		while (x >= 0 && y < HEIGHT&&x + 3 < WIDTH&&y + 3 < HEIGHT) {
			int playerCnt = 0, opponentCnt = 0;
			for (int k = 0; k < 4; ++k) {
				int temp = board[x + k][y + k];
				if (temp == player)++playerCnt;
				else if (temp == opponent) {
					++opponentCnt;
					break;
				}
			}
			++x;
			++y;
			if (opponentCnt != 0)continue;
			else if (4 == playerCnt)score += 1000000;
			else if (3 == playerCnt)score += 30000;
			else if (2 == playerCnt)score += 100;
			else if (1 == playerCnt)score += 50;
		}
	}
	return score;
}

int AlphaBetaPruning::alphaBetaPruning(int depth, int alpha, int beta, int player, int & move)
{
	move = -1;
	++searchedPosition;
	bool findedFlag;
	//获得hash值
	int hashVal = table.probeHash(getKey(), depth, alpha, beta, move, findedFlag);
	int hashFlag = TranspositionTableNode::ALPHA_FLAG;
	//已经搜过了，可以直接返回
	//TranspositionTable::unknown != hashVal || findedFlag
	if (findedFlag) {
		++ttHitCnt;
		return hashVal;
	}
	//先判断下了之后会不会立即胜利
	for (const int &i : columnOrder) {
		if (!canPlay(i))continue;
		if (isWinMove(i, true)) {
			move = i;
			return (INF >> 1) - HEIGHT * WIDTH - round;
		}
	}
	int twoCnt = 0, forcedMove;
	//自己的活二
	for (int i = 0; i < WIDTH; ++i) {
		if (!canPlay(i))continue;
		uint64_t playedMask = ((mask + bottomMask(i)) & columnMask(i));
		uint64_t winTwo = (getWinPositions(bitBoard | playedMask, mask | playedMask)&getAllValidPositions());
		if (0 != (winTwo&(winTwo - 1))) {
			move = i;
			return (INF >> 1) - HEIGHT * WIDTH - round - 2;
		}
	}
	//下一步能下的
	uint64_t nextPosition = getAllNotLosePositions(forcedMove);
	//双迫手
	if (0 == nextPosition) {
		move = forcedMove;
		return (INF >> 1) - HEIGHT * WIDTH - round - 1;
	}
	//对手活二
	twoCnt = 0;
	for (int i = 0; i < WIDTH; ++i) {
		//获得要下的位置
		uint64_t playedMask = (nextPosition&columnMask(i));
		if (0 == playedMask)continue;

		uint64_t opponent = (getWinPositions(((bitBoard^mask) | playedMask), mask | playedMask)&getAllValidPositions());
		//活二下下去就是活3
		if (0 != (opponent & (opponent - 1))) {
			++twoCnt;
			nextPosition = playedMask;
			break;
		}
	}

	//达到搜索深度了
	if (depth <= 0) {
		//得分=自己的-对手的
		int tempVal = evaluate(player) - evaluate(-player);
		table.recordHash(getKey(), tempVal, depth, TranspositionTableNode::EXACT_FLAG, -1);
		return tempVal;
	}

	MoveSorter moveSorter;
	for (int i = WIDTH - 1; i >= 0; --i) {
		uint64_t playedMask = nextPosition & columnMask(columnOrder[i]);
		if (0 == playedMask)continue;
		moveSorter.addMove(columnOrder[i], getScore(playedMask));
	}

	//如果有记录最佳着法
	if (findedFlag&&-1 != move) {
		moveSorter.changeOrder(move);
	}

	int tempMove;
	while (!moveSorter.isEmpty()) {
		int i = moveSorter.getNextMove();
		//if (0 == (nextPosition & columnMask(i)))continue;
		play(i, player);
		++currentDepth;
		int val;
		if (HEIGHT*WIDTH == round)val = 0;
		else val = -alphaBetaPruning(depth - 1, -beta, -alpha, -player, tempMove);
		undo(i);
		--currentDepth;
		if (val >= beta) {
			table.recordHash(getKey(), beta, depth, TranspositionTableNode::BETA_FLAG, i);
			return beta;
		}
		if (val > alpha) {
			hashFlag = TranspositionTableNode::EXACT_FLAG;
			move = i;
			alpha = val;
		}
	}
	table.recordHash(getKey(), depth, alpha, hashFlag, move);
	return alpha;
}

int AlphaBetaPruning::getColByAlphaBetaPruning(const int & player)
{
	int opponentWinMove[2];
	int size = 0;
	for (int i = 0; i < WIDTH; ++i) {
		//下一步就能赢
		if (isWinMove(i, true))return i;
		else if (isWinMove(i, true, true)) {
			if (size < 2) {
				opponentWinMove[size] = i;
				++size;
			}
		}
	}
	//对方至少有一个迫手，就下着了
	if (0 != size)return opponentWinMove[0];

	//table.reset();

	int col;
	ttHitCnt = 0;
	searchedPosition = 0;
	alphaBetaPruning(maxDepth, -INF, INF, player, col);
	_cprintf("hit:%d\r\n", ttHitCnt);
	_cprintf("all:%d\r\n", searchedPosition);
	_cprintf("hit rate:%lf%%\r\n", ttHitCnt * 100.0 / searchedPosition);

	return col;
}

void AlphaBetaPruning::initSocket()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//新建客户端socket
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//定义要连接的服务端地址
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);//连接端口
	addrServer.sin_addr.S_un.S_addr = inet_addr(IP);
	if (connect(sockClient, (SOCKADDR*)&addrServer, sizeof(addrServer)) < 0)
	{
		throw "未连接到服务器\n";
	}
}

void AlphaBetaPruning::closeSocket()
{
	closesocket(sockClient);
	WSACleanup();
}

void AlphaBetaPruning::sendData(const string & msg) const
{
	if (send(sockClient, msg.data(), msg.length(), 0) != SOCKET_ERROR) {
		_cprintf("Client:%s\r\n", msg.data());
	}
	else {
		_cprintf("send error!!!\r\n");
	}
}

string AlphaBetaPruning::recvData() const
{
	char message[Len] = { 0 };
	if (recv(sockClient, message, Len, 0) < 0)
	{
		_cprintf("receive error!!!\r\n");
		return GameOver;
	}
	else
	{
		_cprintf("Server:%s\r\n", message);
		string msg(message);
		return msg.substr(0, msg.find_first_of('\r'));
	}
}

AlphaBetaPruning::AlphaBetaPruning()
{
	for (int i = 0; i < WIDTH; i++) {
		columnOrder[i] = WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
	}
}

void AlphaBetaPruning::printBoard() const
{
	_cprintf("***********************\r\n");
	for (int j = HEIGHT - 1; j >= 0; --j) {
		for (int i = 0; i < WIDTH; ++i) {
			_cprintf("|");
			if (0 == board[i][j])_cprintf("_");
			else if (1 == board[i][j])_cprintf("X");
			else _cprintf("O");
		}
		_cprintf("|\r\n");
	}
	for (int i = 0; i < WIDTH; ++i)_cprintf(" %d", i);
	_cprintf(" \r\n");
	_cprintf("***********************\r\n");
}

bool AlphaBetaPruning::canPlay(const int & col) const
{
	return 0 == (mask & topMask(col));
}

void AlphaBetaPruning::play(const int & col, const int & player)
{
	board[col][currentHeight[col]] = player;
	bitBoard ^= mask;
	mask |= mask + bottomMask(col);
	++currentHeight[col];
	++round;

}

void AlphaBetaPruning::undo(const int & col)
{

	--currentHeight[col];
	board[col][currentHeight[col]] = 0;
	mask &= ~((1ULL << currentHeight[col]) << (col*(HEIGHT + 1)));
	bitBoard ^= mask;
	--round;
}

void AlphaBetaPruning::initPlay(const string & s)
{
	int player = 1;
	for (const char &c : s) {
		play(c - '1', player);
		player = -player;
	}
}

bool AlphaBetaPruning::isWinMove(const int& lastCol, const bool& predict, const bool& isOpponent) const
{
	uint64_t tempBoard = bitBoard;
	//预测
	if (predict) {
		if (isOpponent)tempBoard ^= mask;
		//play之前总是显示自己的，所以将要下的列加上1就好
		tempBoard |= (mask + bottomMask(lastCol))&columnMask(lastCol);
	}
	else {
		//play之后总是显示对手的，所以异或一下显示自己的
		tempBoard ^= mask;
	}
	/*
	垂直
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

	//水平
	//右移height+1相当于移到前一列同一行
	tempMask = tempBoard & (tempBoard >> (HEIGHT + 1));
	if (0 != (tempMask&(tempMask >> ((HEIGHT + 1) << 1))))return true;

	//副对角线
	//右移height+2相当于移到前一列同一行的下一个
	tempMask = tempBoard & (tempBoard >> (HEIGHT + 2));
	if (0 != (tempMask&(tempMask >> ((HEIGHT + 2) << 1))))return true;

	//主对角线
	//右移height+1相当于移到前一列同一行的上一个
	tempMask = tempBoard & (tempBoard >> HEIGHT);
	return (0 != (tempMask&(tempMask >> (HEIGHT << 1))));
}

bool AlphaBetaPruning::isEnd(const int & lastCol, const int & player) const
{
	if (isWinMove(lastCol, false)) {
		if (1 == player)_cprintf("X");
		else _cprintf("O");
		_cprintf("Win\r\n");
		return true;
	}
	else if (HEIGHT*WIDTH == round) {
		_cprintf("draw\r\n");
		return true;
	}
	else return false;
}

void AlphaBetaPruning::pvp()
{
	printBoard();
	int col;
	//是否轮到X
	int player = 1;
	while (true) {
		if (player)_cprintf("X turn\r\n");
		else _cprintf("O turn\r\n");
		col = getCorrectCol();
		play(col, player);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}

void AlphaBetaPruning::pve(const bool & firstFlag)
{
	int col;
	int player = 1;
	printBoard();
	while (true) {
		if (firstFlag) {
			_cprintf("X turn\r\n");
			col = getCorrectCol();
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			_cprintf("O turn\r\n");
			col = getColByAlphaBetaPruning(player);
			_cprintf("%d\r\n", col);
		}
		else {
			_cprintf("X turn\r\n");
			col = getColByAlphaBetaPruning(player);
			_cprintf("%d\r\n", col);
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			_cprintf("O turn\r\n");
			col = getCorrectCol();
		}
		play(col, player);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}

void AlphaBetaPruning::AI_course_socket()
{
	int col;
	int player = 1;
	try
	{
		initSocket();
		sendData(TeamName);
		string msg = recvData();
		int out;
		if (msg == Success) {
			while (true) {
				msg = recvData();
				if (!(msg == GameOver)) {
					if (msg == Begin) {
						col = getColByAlphaBetaPruning(player);
						play(col, player);
						player = -player;
						out = col + 1;
					}
					else {
						int in = stoi(msg);
						play(in - 1, player);
						player = -player;
						col = getColByAlphaBetaPruning(player);
						play(col, player);
						player = -player;
						out = col + 1;
					}
					sendData(to_string(out));
				}
				else {
					break;
				}
			}
		}

		closeSocket();
	}
	catch (exception e)
	{
		printf(e.what());
	}
}

MoveSorter::MoveSorter() :moveSorterSize(0)
{
}

void MoveSorter::addMove(const int & move, const int & score)
{
	int pos = moveSorterSize;
	++moveSorterSize;
	//插入排序
	while (pos > 0 && container[pos - 1].score > score) {
		container[pos] = container[pos - 1];
		--pos;
	}
	container[pos].setData(move, score);
}

int MoveSorter::getNextMove()
{
	--moveSorterSize;
	return container[moveSorterSize].move;
}

void MoveSorter::changeOrder(const int & move)
{
	for (int i = 0; i < AlphaBetaPruning::WIDTH; ++i) {
		if (container[i].move == move) {
			swap(container[i], container[moveSorterSize - 1]);
			break;
		}
	}
}

bool MoveSorter::isEmpty() const
{
	return 0 == moveSorterSize;
}
