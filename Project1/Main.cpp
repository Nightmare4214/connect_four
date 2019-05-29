#include<iostream>
#include<string>
#include<cstring>
using namespace std;
//�ߺͿ�
const int HEIGHT = 6, WIDTH = 7;
//�����
const int INF = 0x3f3f3f3f;
//����,0�����_��1�����(����)X��-1�����O
int board[WIDTH][HEIGHT];
//��ǰÿ���еĸ߶�
int currentHeight[WIDTH];
//���˼�����
int chessCnt;
//����˳��
int columnOrder[WIDTH];
//�������
void printBoard() {
	cout << "***********************" << endl;
	for (int j = HEIGHT - 1; j >= 0; --j) {
		for (int i = 0; i < WIDTH; ++i) {
			cout << '|';
			if (0 == board[i][j])cout << '_';
			else if (1 == board[i][j])cout << 'X';
			else cout << 'O';
		}
		cout << '|' << endl;
	}
	for (int i = 0; i < WIDTH; ++i)cout << ' ' << i;
	cout << ' ' << endl;
	cout << "***********************" << endl;
}
/**
 * ��һ���ܲ��ܷ�����
 * @param col ���ڵڼ���(0-WIDTH-1)
 * @return ��һ���ܲ��ܷ�����
 */
bool canPlay(const int& col) {
	return currentHeight[col] < HEIGHT;
}
/**
 * ������
 * @param col ���ڵڼ���(0-WIDTH-1)
 * @param player �������(1����-1)
 */
void play(const int& col, const int& player) {
	board[col][currentHeight[col]] = player;
	++currentHeight[col];
	++chessCnt;
}
/**
 * ����
 * @param col ���ڵڼ���(0-WIDTH-1)
 */
void undo(const int& col) {
	--currentHeight[col];
	board[col][currentHeight[col]] = 0;
	--chessCnt;
}
void initPlay(const string& s) {
	int player = 1;
	for (const char &c : s) {
		play(c - '0', player);
		player = -player;
	}
}
/**
 * ������һ���ܲ���Ӯ
 * @param col ���ڵڼ���(0-WIDTH-1)
 * @param player �������(1����-1)
 * @return �Ƿ�ʤ��
 */
bool isWinMove(const int& lastCol, const int& player) {
	int cnt = 0;
	if (currentHeight[lastCol] > 3) {
		for (int i = currentHeight[lastCol] - 1; i >= currentHeight[lastCol] - 4; --i) {
			if (player == board[lastCol][i])++cnt;
			else break;
		}
		if (4 == cnt)return true;
	}
	//-1���Խ��ߣ�0�ᣬ1���Խ���
	for (int dy = -1; dy <= 1; ++dy) {
		cnt = 0;
		//������ߣ������ұ�
		for (int dx = -1; dx <= 1; dx += 2) {
			int x = lastCol + dx, y = currentHeight[lastCol] + dx * dy - 1;
			while (x >= 0 && x < WIDTH&&y >= 0 && y < HEIGHT && player == board[x][y]) {
				x += dx;
				y += dx * dy;
				++cnt;
			}
		}
		if (cnt >= 3)return true;
	}
	return false;
}
/**
 * �Ƿ�ʤ�����ߺ���
 * @param lastCol�µ����һ��
 * @param player 1����X,-1����O
 * @return �Ƿ�ʤ�����ߺ���
 */
bool isEnd(const int& lastCol, const int& player) {
	if (isWinMove(lastCol, player)) {
		if (1 == player)cout << "X";
		else cout << "O";
		cout << "�����ʤ��" << endl;
		return true;
	}
	else if (HEIGHT*WIDTH == chessCnt) {
		cout << "ƽ��" << endl;
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
				cout << "�в���ȷ,����������" << endl;
			}
			else if (!canPlay(col)) {
				cout << "��һ���Ѿ�����,����������" << endl;
			}
			else return col;
		}
		catch (exception e) {
			cout << "���벻��ȷ,����������" << endl;
		}
	}
	return 0;
}
//���˶�ս
void pvp() {
	printBoard();
	int col;
	//�Ƿ��ֵ�X
	int player = 1;
	while (true) {
		col = getCorrectCol();
		play(col, player);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}
/**
 * ��������÷֣�����Ϊ���еĿ�λ�÷������Ӻ��м���Ӯ��
 * @param player 1����X,-1����O
 * @return �÷�
 */
int evaluate(const int& player) {
	int opponent = -player;
	int score = 0;
	//���ŵ�
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
	//��ֱ��
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
	//���Խ���y=-x+bias
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
	//���Խ���y=x+bias
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
/**
 * alpha-beta��֦
 * @param depth �������
 * @param alpha
 * @param beta
 * @param player ��ң�1����X,-1����O
 * @param move �µ���
 * @return alphaֵ
 */
int alphaBetaPruning(int depth, int alpha, int beta, int player, int& move) {
	//�ﵽ���������
	if (depth <= 0) {
		//�÷�=�Լ���-���ֵ�
		return evaluate(player) - evaluate(-player);
	}
	int bestMove = 0, tempMove;
	for (const int &i : columnOrder) {
		if (!canPlay(i))continue;
		play(i, player);
		int val;
		if (isWinMove(i, player)) {
			move = i;
			undo(i);
			return INF - HEIGHT * WIDTH + chessCnt;
		}
		else if (HEIGHT*WIDTH == chessCnt)val = 0;
		else val = -alphaBetaPruning(depth - 1, -beta, -alpha, -player, tempMove);
		undo(i);
		if (val >= beta)return beta;
		if (val > alpha) {
			move = i;
			alpha = val;
		}
	}
	return alpha;
}
/**
 * �˻���ս
 * @param firstFlag �����Ƿ�����
 */
void pve(const bool &firstFlag) {
	int col;
	int player = 1;
	printBoard();
	const int maxDepth = 10;
	while (true) {
		if (firstFlag) {
			cout << "�ֵ�X" << endl;
			col = getCorrectCol();
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			cout << "�ֵ�O" << endl;
			alphaBetaPruning(maxDepth, -INF, INF, player, col);
			cout << col << endl;
		}
		else {
			cout << "�ֵ�X" << endl;
			alphaBetaPruning(maxDepth, -INF, INF, player, col);
			cout << col << endl;
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			cout << "�ֵ�O" << endl;
			col = getCorrectCol();
		}
		play(col, player);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}
int main() {
	int temp;
	cout << "ѡ��1���˶�ս��0�˻���ս" << endl;
	bool pvpFlag, firstFlag = true;
	cin >> temp;
	pvpFlag = temp;
	if (pvpFlag)cout << "ѡ�������˶�ս" << endl;
	else cout << "ѡ�����˻���ս" << endl;
	if (!pvpFlag) {
		cout << "ѡ��1���֣�0����" << endl;
		cin >> temp;
		firstFlag = temp;
		if (firstFlag)cout << "ѡ��������" << endl;
		else cout << "ѡ���˺���" << endl;
	}
	cout << endl;
	//���м俪ʼ(����ȡ��),Ȼ������
	for (int i = 0; i < WIDTH; i++) {
		columnOrder[i] = WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
	}

	if (pvpFlag)pvp();
	else pve(firstFlag);
	return 0;
}