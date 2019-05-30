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
#ifdef _DEBUG
	board.printBoard();
#endif // _DEBUG
	int col;
	bool XFlag = true;
	while (true) {
#ifdef _DEBUG
		if (XFlag)cout << "�ֵ�X" << endl;
		else cout << "�ֵ�O" << endl;
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
#ifdef _DEBUG
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
#endif // _DEBUG
			col = getCorrectCol();
			board.play(col);
#ifdef _DEBUG
			board.printBoard();
#endif // _DEBUG
			if (board.isTerminal()) {
				int getResult = board.getResult();
				if (1 == getResult)cout << "X�����ʤ��";
				else if (-1 == getResult)cout << "O�����ʤ��";
				else cout << "ƽ��" << endl;
				return;
			}
			XFlag = !XFlag;
#ifdef _DEBUG
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
#endif // _DEBUG
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
		}
		else {
#ifdef _DEBUG
			if (XFlag)cout << "�ֵ�X" << endl;
			else cout << "�ֵ�O" << endl;
#endif // _DEBUG
			solver.init(board);
			col = solver.getColByUCT();
			cout << col << endl;
			board.play(col);
#ifdef _DEBUG
			board.printBoard();
#endif // _DEBUG
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
#ifdef _DEBUG
		board.printBoard();
#endif // _DEBUG
		if (board.isTerminal()) {
			int getResult = board.getResult();
			if (1 == getResult)cout << "X�����ʤ��";
			else if (-1 == getResult)cout << "O�����ʤ��";
			else cout << "ƽ��" << endl;
			return;
		}
	}
}
