#include"ConnectFour.hpp"
#include<iostream>
#include<string>
using namespace std;
int ConnectFour::getCorrectCol()
{
	string temp;
	int col;
	while (cin >> temp) {
		try {
			col = stoi(temp);
			if (col < 0 || col >= ConnectFourState::WIDTH) {
				cout << "列不正确,请重新输入" << endl;
			}
			else if (!board.canPlay(col)) {
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

void ConnectFour::initSocket()
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

void ConnectFour::closeSocket()
{
	closesocket(sockClient);
	WSACleanup();
}

void ConnectFour::sendData(const string & msg) const
{
	if (SOCKET_ERROR != send(sockClient, msg.data(), msg.length(), 0))
		printf("Client:%s\n", msg.data());
	else
		printf("发送失败!\n");
}

string ConnectFour::recvData() const
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

void ConnectFour::pvp()
{
	board.printBoard();
	int col;
	bool XFlag = true;
	while (true) {
		if (XFlag)cout << "轮到X" << endl;
		else cout << "轮到O" << endl;
		col = getCorrectCol();
		board.play(col);
		board.printBoard();
		XFlag = !XFlag;
		if (board.isTerminal())return;
	}
}

void ConnectFour::pve(const bool & playerFirst)
{
	bool XFlag = true;
	board.printBoard();
	int col;
	while (true) {
		if (playerFirst) {
			if (XFlag)cout << "轮到X" << endl;
			else cout << "轮到O" << endl;
			col = getCorrectCol();
			board.play(col);
			board.printBoard();
			if (board.isTerminal()) {
				int getResult = board.getResult();
				if (1 == getResult)cout << "X获得了胜利";
				else if (-1 == getResult)cout << "O获得了胜利";
				else cout << "平局" << endl;
				return;
			}
			XFlag = !XFlag;
			if (XFlag)cout << "轮到X" << endl;
			else cout << "轮到O" << endl;
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
		}
		else {
			if (XFlag)cout << "轮到X" << endl;
			else cout << "轮到O" << endl;
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
			board.play(col);
			board.printBoard();
			if (board.isTerminal()) {
				int getResult = board.getResult();
				if (1 == getResult)cout << "X获得了胜利";
				else if (-1 == getResult)cout << "O获得了胜利";
				else cout << "平局" << endl;
				return;
			}
			XFlag = !XFlag;
			col = getCorrectCol();
		}
		board.play(col);
		board.printBoard();
		if (board.isTerminal()) {
			int getResult = board.getResult();
			if (1 == getResult)cout << "X获得了胜利";
			else if (-1 == getResult)cout << "O获得了胜利";
			else cout << "平局" << endl;
			return;
		}
	}
}

void ConnectFour::AI_course_socket()
{
	//board.printBoard();
	int col;
	try
	{
		initSocket();

		random_device rd;

		sendData(TeamName);

		string msg = recvData();
		int out;
		if (msg == Success) {
			while (true) {
				msg = recvData();

				if (!(msg == GameOver)) {

					if (msg == Begin) {
						solver.init(board);
						col = solver.getColByUCT();
						board.play(col);
						//board.printBoard();
						out = col + 1;
						//out = 1 + rd() % 8;
					}
					else {
						int in = stoi(msg);
						board.play(in - 1);
						solver.init(board);
						col = solver.getColByUCT();
						board.play(col);
						//board.printBoard();
						out = col + 1;
						//out = out = 1 + rd() % 8;
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
