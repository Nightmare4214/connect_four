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
//高和宽
const int HEIGHT = 6, WIDTH = 8;
//最大深度
const int maxDepth = 10;
//无穷大
const int INF = 0x3f3f3f3f;
//棋盘,0代表空_，1代表白(先手)X，-1代表黑O
int board[WIDTH][HEIGHT];
//当前每个列的高度
int currentHeight[WIDTH];
//下了几个子
int chessCnt;
//下棋顺序
int columnOrder[WIDTH];

WSADATA wsaData;
//客户端Socket
SOCKET sockClient;
//服务端地址
SOCKADDR_IN addrServer;
static const int Len = 1024;
//服务端ip
const char *IP = "127.0.0.1";
//服务端的端口
const int PORT = 6000;
//成功连接
const string Success = "success";
//游戏开始
const string Begin = "begin";
//游戏结束
const string GameOver = "game_over";
//队名
const string TeamName = "faQ";
/**
 * 输出棋盘
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
 * 这一列能不能放棋子
 * @param col 放在第几列(0-WIDTH-1)
 * @return 这一列能不能放棋子
 */
bool canPlay(const int& col) {
	return currentHeight[col] < HEIGHT;
}
/**
 * 放棋子
 * @param col 放在第几列(0-WIDTH-1)
 * @param player 玩家类型(1或者-1)
 */
void play(const int& col, const int& player) {
	board[col][currentHeight[col]] = player;
	++currentHeight[col];
	++chessCnt;
}
/**
 * 悔棋
 * @param col 放在第几列(0-WIDTH-1)
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
 * 下完这一步能不能赢
 * @param col 放在第几列(0-WIDTH-1)
 * @param player 玩家类型(1或者-1)
 * @return 是否胜利
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
	//-1主对角线，0横，1副对角线
	for (int dy = -1; dy <= 1; ++dy) {
		cnt = 0;
		//先算左边，再算右边
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
 * 是否胜利或者和棋
 * @param lastCol下的最后一列
 * @param player 1代表X,-1代表O
 * @return 是否胜利或者和棋
 */
bool isEnd(const int& lastCol, const int& player) {
	if (isWinMove(lastCol, player)) {
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
/**
 * 获得玩家输入的正确的列
 * @return 正确的列
 */
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
/**
 * 人人对战
 */
void pvp() {
	printBoard();
	int col;
	//是否轮到X
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
 * 评估局面得分：规则为所有的空位置放上棋子后有几种赢法
 * @param player 1代表X,-1代表O
 * @return 得分
 */
int evaluate(const int& player) {
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
/**
 * alpha-beta剪枝
 * @param depth 搜索深度
 * @param alpha
 * @param beta
 * @param player 玩家，1代表X,-1代表O
 * @param move 下的列
 * @return alpha值
 */
int alphaBetaPruning(int depth, int alpha, int beta, int player, int& move) {
	//达到搜索深度了
	if (depth <= 0) {
		//得分=自己的-对手的
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
 * 人机对战
 * @param firstFlag 人类是否先手
 */
void pve(const bool &firstFlag) {
	int col;
	int player = 1;
	printBoard();
	while (true) {
		if (firstFlag) {
			cout << "轮到X" << endl;
			col = getCorrectCol();
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			cout << "轮到O" << endl;
			alphaBetaPruning(maxDepth, -INF, INF, player, col);
			cout << col << endl;
		}
		else {
			cout << "轮到X" << endl;
			alphaBetaPruning(maxDepth, -INF, INF, player, col);
			cout << col << endl;
			play(col, player);
			printBoard();
			if (isEnd(col, player))return;
			player = -player;
			cout << "轮到O" << endl;
			col = getCorrectCol();
		}
		play(col, player);
		printBoard();
		if (isEnd(col, player))return;
		player = -player;
	}
}

/**
 * 初始化socket
 */
void initSocket()
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
/**
 * 关闭socket
 */
void closeSocket()
{
	closesocket(sockClient);
	WSACleanup();
}
/**
 * 发送消息
 * @param msg 发送的消息
 */
void sendData(const string& msg)
{
	if (send(sockClient, msg.data(), msg.length(), 0) != SOCKET_ERROR)
		printf("Client:%s\n", msg.data());
	else
		printf("发送失败!\n");
}
/**
 * 接收消息
 * @return 接收到的消息
 */
string recvData()
{
	char message[Len] = { 0 };
	if (recv(sockClient, message, Len, 0) < 0)
	{
		printf("接收失败!\n");
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
	//    cout << "选择1人人对战，0人机对战" << endl;
	//    bool pvpFlag, firstFlag = true;
	//    cin >> temp;
	//    pvpFlag = temp;
	//    if (pvpFlag)cout << "选择了人人对战" << endl;
	//    else cout << "选择了人机对战" << endl;
	//    if (!pvpFlag) {
	//        cout << "选择1先手，0后手" << endl;
	//        cin >> temp;
	//        firstFlag = temp;
	//        if (firstFlag)cout << "选择了先手" << endl;
	//        else cout << "选择了后手" << endl;
	//    }
	//    cout << endl;
	//    //从中间开始(向下取整),然后左右
	//    for (int i = 0; i < WIDTH; i++) {
	//        columnOrder[i] = WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
	//    }
	//
	//    if (pvpFlag)pvp();
	//    else pve(firstFlag);
	return 0;
}