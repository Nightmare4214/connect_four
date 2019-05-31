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
				cout << "�в���ȷ,����������" << endl;
			}
			else if (!board.canPlay(col)) {
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

void ConnectFour::pvp()
{
	board.printBoard();
	int col;
	bool XFlag = true;
	while (true) {
		if (XFlag)cout << "�ֵ�X" << endl;
		else cout << "�ֵ�O" << endl;
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
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
			col = getCorrectCol();
			board.play(col);
			board.printBoard();
			if (board.isTerminal()) {
				int getResult = board.getResult();
				if (1 == getResult)cout << "X�����ʤ��";
				else if (-1 == getResult)cout << "O�����ʤ��";
				else cout << "ƽ��" << endl;
				return;
			}
			XFlag = !XFlag;
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
		}
		else {
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
			board.play(col);
			board.printBoard();
			if (board.isTerminal()) {
				int getResult = board.getResult();
				if (1 == getResult)cout << "X�����ʤ��";
				else if (-1 == getResult)cout << "O�����ʤ��";
				else cout << "ƽ��" << endl;
				return;
			}
			XFlag = !XFlag;
			col = getCorrectCol();
		}
		board.play(col);
		board.printBoard();
		if (board.isTerminal()) {
			int getResult = board.getResult();
			if (1 == getResult)cout << "X�����ʤ��";
			else if (-1 == getResult)cout << "O�����ʤ��";
			else cout << "ƽ��" << endl;
			return;
		}
	}
}
