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

void ConnectFour::pvp()
{
#ifdef _DEBUG
	board.printBoard();
#endif // _DEBUG
	int col;
	bool XFlag = true;
	while (true) {
#ifdef _DEBUG
		if (XFlag)cout << "轮到X" << endl;
		else cout << "轮到O" << endl;
#endif // _DEBUG
		col = getCorrectCol();
		board.play(col);
#ifdef _DEBUG
		board.printBoard();
		XFlag = !XFlag;
#endif // _DEBUG
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
			if (XFlag)cout << "轮到X" << endl;
			else cout << "轮到O" << endl;
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

