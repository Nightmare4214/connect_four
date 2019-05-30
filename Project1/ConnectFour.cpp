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
	while (true) {
		col = getCorrectCol();
		board.play(col);
#ifdef _DEBUG
		board.printBoard();
#endif // _DEBUG
		if (board.isTerminal())return;
	}
}
