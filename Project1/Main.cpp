#include<iostream>
#include<string>
#include<cstring>
using namespace std;
//高和宽
const int HEIGHT = 6, WIDTH = 8;
//无穷大
const int INF = 0x3f3f3f3f;
/*位棋盘，只存当前玩家的棋子
HEIGHT+1位存1个列(最高的用来表示满了)
通过^掩码来改变当前玩家的棋子
*/
uint64_t board;
/*
掩码,存所有的棋子，用1表示
通过在列的最底下+1后或运算来下棋
*/
uint64_t mask;
int tempBoard[WIDTH][HEIGHT];
void toTempBoard() {
	char player = 'X', opponent = 'O';
	int player = 1, opponent = -1;
	if (0 == (chessCnt & 1))swap(player, opponent);
	uint64_t tempBitBoard = board;
	uint64_t tempMask = mask;
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			uint64_t a = tempBitBoard & 1;
			uint64_t b = tempMask & 1;
			if (0 == b)tempBoard[i][j] = 0;
			else if()
		}
		tempBitBoard >>= 1;
		tempMask >>= 1;
	}
}
//移动步骤
int chessCnt;
//下棋顺序
int columnOrder[WIDTH];
//输出棋盘
void printBoard() {
	char player = 'X', opponent = 'O';
	if (0 == (chessCnt & 1))swap(player, opponent);
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
//移动到col列最高的位置
uint64_t topMask(const int &col) {
	/*
	1ULL << (HEIGHT - 1)将1移到HEIGHT-1的位置(也就是棋盘的最高)
	然后移到第col列
	*/
	return (1ULL << (HEIGHT - 1)) << ((HEIGHT + 1)*col);
}
/**
 * 这一列能不能放棋子
 * @param col 放在第几列(0-WIDTH-1)
 * @return 这一列能不能放棋子
 */
bool canPlay(const int &col) {
	return 0 == (mask & topMask(col));
}
//移动到col列最底下
uint64_t bottomMask(const int &col) {
	return 1ULL << ((HEIGHT + 1)*col);
}
/**
 * 放棋子
 * @param col 放在第几列(0-WIDTH-1)
 */
void play(const int &col) {
	//board转换成对手的棋子
	board ^= mask;
	//mask在col列加一个子
	mask |= mask + bottomMask(col);
	++chessCnt;
}
/**
 * 悔棋
 * @param col 放在第几列(0-WIDTH-1)
 */
void undo(const int &col) {
	uint64_t temp = topMask(col);
	for (int i = 0; i < HEIGHT; ++i) {
		if (0 != (temp & mask)) {
			mask &= ~temp;
			board ^= mask;
			--chessCnt;
			return;
		}
		else {
			temp >>= 1;
		}
	}
}
//判断这一步下下去能不能赢（play之后调用）
bool isWinMove(const int &col) {
	//切换成自己的
	uint64_t tempBoard = board ^ mask;
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

	//主对角线
	//右移height+2相当于移到前一列同一行的下一个
	tempMask = tempBoard & (tempBoard >> (HEIGHT + 2));
	if (0 != (tempMask&(tempMask >> ((HEIGHT + 2) << 1))))return true;

	//副对角线
	//右移height+1相当于移到前一列同一行的上一个
	tempMask = tempBoard & (tempBoard >> HEIGHT);
	return 0 != (tempMask&(tempMask >> (HEIGHT << 1)));
}
/**
 * 是否胜利或者和棋
 * @param lastCol下的最后一列
 * @param player 1代表X,-1代表O
 * @return 是否胜利或者和棋
 */
bool isEnd(const int& lastCol, const int& player) {
	if (isWinMove(lastCol)) {
		if (1 == player)cout << "X";
		else cout << "O";
		cout << "获得了胜利" << endl;
		return true;
	}
	else if (HEIGHT*WIDTH == chessCnt) {
		cout << "平局" << endl;
		return true;
	}
	else return false;
}
int getCorrectCol() {
	string temp;
	int col;
	while (cin >> temp) {
		try {
			col = stoi(temp);
			if (col < 0 || col >= WIDTH) {
				cout << "列不正确,请重新输入" << endl;
			}
			else if (!canPlay(col)) {
				cout << "这一列已经满了,请重新输入" << endl;
			}
			else return col;
		}
		catch (exception e) {
			cout << "输入不正确,请重新输入" << endl;
		}
	}
	return 0;
}
//人人对战
void pvp() {
	printBoard();
	int col;
	//是否轮到X
	int player = 1;
	while (true) {
		col = getCorrectCol();
		play(col);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}
/**
 * 评估局面得分：规则为所有的空位置放上棋子后有几种赢法
 * @param player 1代表X,-1代表O
 * @return 得分
 */
int evaluate(const int& player) {
	int player = 1, opponent = -1;
	if (0 == (chessCnt & 1))swap(player, opponent);
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
int main() {
	pvp();
	return 0;
}
