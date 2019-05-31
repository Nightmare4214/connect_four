//#include"MCTSNode.hpp"
//#include"ConnectFour.hpp"
//int main() {
//	ConnectFour t;
//	/*
//	t.pvp();
//	t.pve(true);
//	t.pve(false);*/
//	t.AI_course_socket();
//	return 0;
//}
#pragma comment(lib,"ws2_32.lib")
#include<Winsock2.h>
#include<Ws2tcpip.h>
#include<cstdio>
#include<iostream>
#include<random>
#include<string>
using namespace std;
//�ߺͿ�
const int HEIGHT = 6, WIDTH = 8;
//������
const int maxDepth = 10;
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

WSADATA wsaData;
//�ͻ���Socket
SOCKET sockClient;
//����˵�ַ
SOCKADDR_IN addrServer;
static const int Len = 1024;
//�����ip
const char *IP = "127.0.0.1";
//����˵Ķ˿�
const int PORT = 6000;
//�ɹ�����
const string Success = "success";
//��Ϸ��ʼ
const string Begin = "begin";
//��Ϸ����
const string GameOver = "game_over";
//����
const string TeamName = "faQ";
/**
 * �������
 */
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
/**
 * �������������ȷ����
 * @return ��ȷ����
 */
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
/**
 * ���˶�ս
 */
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
	int tempMove;
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

/**
 * ��ʼ��socket
 */
void initSocket()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�½��ͻ���socket
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//����Ҫ���ӵķ���˵�ַ
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);//���Ӷ˿�
	addrServer.sin_addr.S_un.S_addr = inet_addr(IP);
	if (connect(sockClient, (SOCKADDR*)&addrServer, sizeof(addrServer)) < 0)
	{
		throw "δ���ӵ�������\n";
	}
}
/**
 * �ر�socket
 */
void closeSocket()
{
	closesocket(sockClient);
	WSACleanup();
}
/**
 * ������Ϣ
 * @param msg ���͵���Ϣ
 */
void sendData(const string& msg)
{
	if (send(sockClient, msg.data(), msg.length(), 0) != SOCKET_ERROR)
		printf("Client:%s\n", msg.data());
	else
		printf("����ʧ��!\n");
}
/**
 * ������Ϣ
 * @return ���յ�����Ϣ
 */
string recvData()
{
	char message[Len] = { 0 };
	if (recv(sockClient, message, Len, 0) < 0)
	{
		printf("����ʧ��!\n");
		return GameOver;
	}
	else
	{
		printf("Server:%s\n", message);
		string msg(message);
		return msg.substr(0, msg.find_first_of('\r'));
	}
}
void AI_course_socket() {
	int col;
	int player = 1;
	printBoard();
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
						alphaBetaPruning(maxDepth, -INF, INF, player, col);
						play(col, player);
						player = -player;
						out = col + 1;
					}
					else {
						int in = stoi(msg);
						play(in - 1, player);
						player = -player;
						alphaBetaPruning(maxDepth, -INF, INF, player, col);
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
int main() {
	for (int i = 0; i < WIDTH; i++) {
		columnOrder[i] = WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
    }
	AI_course_socket();
	//    int temp;
	//    cout << "ѡ��1���˶�ս��0�˻���ս" << endl;
	//    bool pvpFlag, firstFlag = true;
	//    cin >> temp;
	//    pvpFlag = temp;
	//    if (pvpFlag)cout << "ѡ�������˶�ս" << endl;
	//    else cout << "ѡ�����˻���ս" << endl;
	//    if (!pvpFlag) {
	//        cout << "ѡ��1���֣�0����" << endl;
	//        cin >> temp;
	//        firstFlag = temp;
	//        if (firstFlag)cout << "ѡ��������" << endl;
	//        else cout << "ѡ���˺���" << endl;
	//    }
	//    cout << endl;
	//    //���м俪ʼ(����ȡ��),Ȼ������
	//    for (int i = 0; i < WIDTH; i++) {
	//        columnOrder[i] = WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
	//    }
	//
	//    if (pvpFlag)pvp();
	//    else pve(firstFlag);
	return 0;
}