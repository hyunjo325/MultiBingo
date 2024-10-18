#pragma once
#include "BingoBoard.h"
BingoBoard::BingoBoard(const int& size) :size(size)
{
	//빙고 배열 생성
	board = make_unique<unique_ptr<Bnumber[]>[]>(size);
	for (int i = 0; i < size; i++) {
		board[i] = make_unique<Bnumber[]>(size);
	}
	int k = 1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j].number = k++;
			board[i][j].check = 0;
		}
	}
	//빙고 배열 섞기
	random_device rd;

	mt19937 genNum(rd());

	uniform_int_distribution<int> ranNum(0, size-1);
	int temp = 0;
	int rdRow;
	int rdCol;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			rdRow = ranNum(genNum);
			rdCol = ranNum(genNum);
			temp = board[rdRow][rdCol].number;
			board[rdRow][rdCol].number = board[i][j].number;
			board[i][j].number = temp;
		}		
	}
}


//빙고판 출력 함수
void BingoBoard::printBoard(const int& locate)
{
	if (locate == 0) {
		gotoxy(4, 4);
		cout << "User";
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; j++) {
				gotoxy(4 + i * 4, 6 + j * 2);
				if (board[i][j].check > 0) {
					if (board[i][j].check % 2 == 0) {
						//컴퓨터가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					}
					else {
						//유저가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					}
				}
				cout << board[i][j].number;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
	else {
		gotoxy(100-6, 4);
		cout << "Com";
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				gotoxy(100 - size * 4 + i * 4, 6 + j * 2);
				if (board[i][j].check > 0) {
					if (board[i][j].check % 2 == 0) {
						//컴퓨터가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					}
					else {
						//유저가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					}
				}
				cout << board[i][j].number;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
}

//유저가 번호 선택하는 함수
int BingoBoard::chooseNum(int& count)
{
	char input = NULL;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	if (y == -1) {
		gotoxy(20, 1);
		cout << "┌────────────┐";
		gotoxy(20, 2);
		cout << "│  저장하기  │";
		gotoxy(20, 3);
		cout << "└────────────┘";
	}
	else {
		gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
		cout << "┌───┐";
		gotoxy(4 + x * 4 - 2, 6 + y * 2);
		cout << "│";
		gotoxy(4 + x * 4 + 2, 6 + y * 2);
		cout << "│";
		gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
		cout << "└───┘ ";
	}
	while (input != 'e') {

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		if (y == -1) {
			gotoxy(20, 1);
			cout << "┌────────────┐";
			gotoxy(20, 2);
			cout << "│  저장하기  │";
			gotoxy(20, 3);
			cout << "└────────────┘";
		}
		else {
			gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
			cout << "      ";
			gotoxy(4 + x * 4 - 2, 6 + y * 2);
			cout << "  ";
			gotoxy(4 + x * 4 + 2, 6 + y * 2);
			cout << "  ";
			gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
			cout << "      ";
		}
		switch (input) {
		case 'l': if (y > -1) x--;break;
		case 'r': if (y > -1) x++; break;
		case 'u':y--; break;
		case 'd':y++; break;
		}
		if (x < 0) {
			x = 0;
		}
		else if (x >= size) {
			x = size - 1;
		}
		if (y < -1) {
			y = -1;
		}
		else if (y >= size) {
			y = size - 1;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		if (y == -1) {
			gotoxy(20, 1);
			cout << "┌────────────┐";
			gotoxy(20, 2);
			cout << "│  저장하기  │";
			gotoxy(20, 3);
			cout << "└────────────┘";
		}
		else {

			gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
			cout << "┌───┐";
			gotoxy(4 + x * 4 - 2, 6 + y * 2);
			cout << "│";
			gotoxy(4 + x * 4 + 2, 6 + y * 2);
			cout << "│";
			gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
			cout << "└───┘ ";
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		if (input == 'e') {
			if (board[x][y].check > 0) {
				input = NULL;
				continue;
			}
		}
	}
	gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
	cout << "      ";
	gotoxy(4 + x * 4 - 2, 6 + y * 2);
	cout << "  ";
	gotoxy(4 + x * 4 + 2, 6 + y * 2);
	cout << "  ";
	gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
	cout << "      ";
	if (y == -1) {
		return -1;
	}
	else {
		board[x][y].check = ++count;
		return board[x][y].number;
	}
	
}

//컴퓨터가 번호 선택하는 함수
int BingoBoard::ComChooseNum(int& count)
{
	unique_ptr<int[]> ComRow = make_unique<int[]>(size);
	unique_ptr<int[]> ComCol = make_unique<int[]>(size);
	unique_ptr<int[]> ComRow0 = make_unique<int[]>(size);
	unique_ptr<int[]> ComCol0 = make_unique<int[]>(size);
	for (int i = 0; i < size; i++) {
		ComRow[i] = 0;
		ComCol[i] = 0;
		ComRow0[i] = 0;
		ComCol0[i] = 0;
	}
	int ComD1 = 0;
	int ComD2 = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j].check > 0) {
				ComRow[i]++;
				ComRow0[i]++;
				ComCol[j]++;
				ComCol0[j]++;
				if (i == j) 
					ComD1++;
				if (i + j == size - 1)
					ComD2++;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		if (ComRow[i] >= size) {
			ComRow[i] = 0;
		}
		if (ComRow0[i] >= size) {
			ComRow0[i] = 0;
		}
		if (ComCol[i] >= size) {
			ComCol[i] = 0;
		}
		if (ComCol0[i] >= size) {
			ComCol0[i] = 0;
		}
		if (ComD1 >= size) {
			ComD1 = 0;
		}
		if (ComD2 >= size) {
			ComD2 = 0;
		}
	}
	
	unique_ptr<int[]> ComRowIdx = make_unique<int[]>(size);
	unique_ptr<int[]> ComColIdx = make_unique<int[]>(size);
	for (size_t i = 0; i < size; i++) {
		ComRowIdx[i] = i;
		ComColIdx[i] = i;
	}
	int temp;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size -1 - i; j++) {
			if (ComRow[j] < ComRow[j + 1]) {
				temp = ComRow[j];
				ComRow[j] = ComRow[j + 1];
				ComRow[j + 1] = temp;

				temp = ComRowIdx[j];
				ComRowIdx[j] = ComRowIdx[j + 1];
				ComRowIdx[j + 1] = temp;
			}
			if (ComCol[j] < ComCol[j + 1]) {
				temp = ComCol[j];
				ComCol[j] = ComCol[j + 1];
				ComCol[j + 1] = temp;

				temp = ComColIdx[j];
				ComColIdx[j] = ComColIdx[j + 1];
				ComColIdx[j + 1] = temp;
			}
		}
	}

	if (ComRow[0] == size - 1) {
		for (size_t i = 0; i < size; i++) {
			if (board[ComRowIdx[0]][i].check == 0) {
				board[ComRowIdx[0]][i].check = ++count;
				return board[ComRowIdx[0]][i].number;
			}
		}
	}
	else if (ComCol[0] == size - 1) {
		for (size_t i = 0; i < size; i++) {
			if (board[i][ComColIdx[0]].check == 0) {
				board[i][ComColIdx[0]].check = ++count;
				return board[i][ComColIdx[0]].number;
			}
		}
	}
	else if (ComD1 == size - 1) {
		for (size_t i = 0; i < size; i++) {
			if (board[i][i].check == 0) {
				board[i][i].check = ++count;
				return board[i][i].number;
			}
		}
	}
	else if (ComD2 == size - 1) {
		for (size_t i = 0; i < size; i++) {
			if (board[i][size - 1 - i].check == 0) {
				board[i][size - 1 - i].check = ++count;
				return board[i][size - 1 - i].number;
			}
		}
	}
	else {
		if (max(max(ComRow[0], ComCol[0]), max(ComD1, ComD2)) == ComRow[0]) {
			if (ComCol[0] >= max(ComD1, ComD2)) {
				for (size_t i = 0; i < size; i++) {
					if (board[ComRowIdx[0]][ComColIdx[i]].check == 0) {
						board[ComRowIdx[0]][ComColIdx[i]].check = ++count;
						return board[ComRowIdx[0]][ComColIdx[i]].number;
					}
				}
			}
			else {
				if (ComD1 + ComCol0[ComRowIdx[0]] > ComD2 + ComCol0[size - ComRowIdx[0] - 1]) {
					for (size_t i = 0; i < size; i++) {
						if (board[ComRowIdx[i]][ComRowIdx[i]].check == 0) {
							board[ComRowIdx[i]][ComRowIdx[i]].check = ++count;
							return board[ComRowIdx[i]][ComRowIdx[i]].number;
						}
					}
				}
				else {
					for (size_t i = 0; i < size; i++) {
						if (board[ComRowIdx[i]][size - ComRowIdx[i] - 1].check == 0) {
							board[ComRowIdx[i]][size - ComRowIdx[i] - 1].check = ++count;
							return board[ComRowIdx[i]][size - ComRowIdx[i] - 1].number;
						}
					}
				}
			}
		}
		else if (max(max(ComRow[0], ComCol[0]), max(ComD1, ComD2)) == ComCol[0]) {
			if (ComRow[0] >= max(ComD1, ComD2)) {
				for (size_t i = 0; i < size; i++) {
					if (board[ComRowIdx[i]][ComColIdx[0]].check == 0) {
						board[ComRowIdx[i]][ComColIdx[0]].check = ++count;
						return board[ComRowIdx[i]][ComColIdx[0]].number;
					}
				}
			}
			else {
				if (ComD1 + ComRow0[ComColIdx[0]] > ComD2 + ComRow0[size - ComColIdx[0] - 1]) {
					for (size_t i = 0; i < size; i++) {
						if (board[ComColIdx[i]][ComColIdx[i]].check == 0) {
							board[ComColIdx[i]][ComColIdx[i]].check = ++count;
							return board[ComColIdx[i]][ComColIdx[i]].number;
						}
					}
				}
				else {
					for (size_t i = 0; i < size; i++) {
						if (board[ComColIdx[i]][size - ComColIdx[i] - 1].check == 0) {
							board[ComColIdx[i]][size - ComColIdx[i] - 1].check = ++count;
							return board[ComColIdx[i]][size - ComColIdx[i] - 1].number;
						}
					}
				}
			}
		}
		else {
			unique_ptr<int[]> ComD1RC = make_unique<int[]>(size);
			unique_ptr<int[]> ComD2RC = make_unique<int[]>(size);
			unique_ptr<int[]> ComD1RCIdx = make_unique<int[]>(size);
			unique_ptr<int[]> ComD2RCIdx = make_unique<int[]>(size);
			for (size_t i = 0; i < size; i++) {
				ComD1RCIdx[i] = i;
				ComD2RCIdx[i] = i;
			}
			for (size_t i = 0; i < size; i++) {
				ComD1RC[i] = ComRow[i] + ComCol[i];
				ComD2RC[i] = ComRow[i] + ComCol[size - i - 1];
			}
			for (size_t i = 0; i < size; i++) {
				for (size_t j = 0; j < size - 1 - i; j++) {
					if (ComD1RC[j] < ComD1RC[j + 1]) {
						temp = ComD1RC[j];
						ComD1RC[j] = ComD1RC[j + 1];
						ComD1RC[j + 1] = temp;

						temp = ComD1RCIdx[j];
						ComD1RCIdx[j] = ComD1RCIdx[j + 1];
						ComD1RCIdx[j + 1] = temp;
					}
					if (ComD2RC[j] < ComD2RC[j + 1]) {
						temp = ComD2RC[j];
						ComD2RC[j] = ComD2RC[j + 1];
						ComD2RC[j + 1] = temp;

						temp = ComD2RCIdx[j];
						ComD2RCIdx[j] = ComD2RCIdx[j + 1];
						ComD2RCIdx[j + 1] = temp;
					}
				}
			}
			if (ComD1 >= ComD2) {
				for (size_t i = 0; i < size; i++) {
					if (board[ComD1RCIdx[i]][ComD1RCIdx[i]].check == 0) {
						board[ComD1RCIdx[i]][ComD1RCIdx[i]].check = ++count;
						return board[ComD1RCIdx[i]][ComD1RCIdx[i]].number;
					}
				}
			}
			else {
				for (size_t i = 0; i < size; i++) {
					if (board[ComD2RCIdx[i]][ComD2RCIdx[i]].check == 0) {
						board[ComD2RCIdx[i]][ComD2RCIdx[i]].check = ++count;
						return board[ComD2RCIdx[i]][ComD2RCIdx[i]].number;
					}
				}
			}
		}
	}

	
	return -1;
}

//입력한 번호를 빙고판에 체크하는 함수
void BingoBoard::checkNum(int num, int& count)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j].number == num) {
				board[i][j].check = count;
				return;
			}
		}
	}
}

//승리했는지 확인하는 함수
int BingoBoard::checkWin()
{
	int line = 0;
	unique_ptr<int[]> ComRow = make_unique<int[]>(size);
	unique_ptr<int[]> ComCol = make_unique<int[]>(size);
	for (int i = 0; i < size; i++) {
		ComRow[i] = 0;
		ComCol[i] = 0;
	}
	int ComD1 = 0;
	int ComD2 = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j].check > 0) {
				ComRow[i]++;
				ComCol[j]++;
				if (i == j)
					ComD1++;
				if (i + j == size - 1)
					ComD2++;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		if (ComRow[i]>=size) {
			line++;
		}
		if (ComCol[i] >= size) {
			line++;
		}
	}
	if (ComD1 >= size) {
		line++;
	}
	if (ComD2 >= size) {
		line++;
	}
	if (line == size * 2 + 2)
		return -1;
	return line;
}

//빙고판을 파일로 저장하는 함수
void BingoBoard::saveBoard(int num, int& count)
{
	const char* file;
	//num이 0이면 user, 1이면 com
	if (!num) {
		file = "./userdata.dat";

	}
	else {
		file = "./comdata.dat";
	}

	ofstream fout;
	fout.open(file, ios::out | ios::binary);
	if (!fout) {
		return;
	}
	fout.write((char*)(&count), sizeof(count));
	fout.write((char*)(&size), sizeof(size));
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			fout.write((char*)(&board[i][j].number), sizeof(board[i][j].number));
			fout.write((char*)(&board[i][j].check), sizeof(board[i][j].check));
		}
	}
	fout.close();
}

//파일에서 불러오는 함수
void BingoBoard::resumeBoard(int num)
{
	const char* file;
	int size;
	int count;
	//num이 0이면 user, 1이면 com
	if (!num) {
		file = "./userdata.dat";
	}
	else {
		file = "./comdata.dat";
	}
	ifstream fin(file, ios::in | ios::binary);
	if (!fin) {
		cout << "파일 열기 오류";
		return;
	}
	fin.read((char*)(&count), sizeof(count));
	fin.read((char*)(&size), sizeof(size));
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			fin.read((char*)(&board[i][j].number), sizeof(board[i][j].number));
			fin.read((char*)(&board[i][j].check), sizeof(board[i][j].check));
		}
	}
	fin.close();
}

//진행상황 다시 보여주는 함수
void BingoBoard::replayBoard()
{
	gotoxy(4, 4);
	cout << "User";
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			gotoxy(4 + i * 4, 6 + j * 2);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			cout << board[i][j].number;
		}
	}
	bool flag = false;
	for (int k = 1; k <= size * size; k++) {
		flag = false;
		for (int i = 0; i < size; ++i) {
			if (flag) break;
			for (int j = 0; j < size; j++) {
				gotoxy(4 + i * 4, 6 + j * 2);
				if (board[i][j].check == k) {
					if (board[i][j].check % 2 == 0) {
						//컴퓨터가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						Sleep(300);
					}
					else {
						//유저가 부른 숫자
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
						Sleep(300);
					}
					cout << board[i][j].number;
					flag = true;
					break;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
	
}

//키보드 입력값을 받는 함수
char BingoBoard::inputKeycode()
{
	char returnValue = NULL;
	while (true) {
		if (_kbhit()) {
			int keycode = _getch();
			if (keycode == 13) {
				returnValue = 'e';
				break;
			}
			if (keycode == 224) {
				keycode = _getch();
				switch (keycode) {
				case 75: returnValue = 'l'; break;
				case 77: returnValue = 'r'; break;
				case 72: returnValue = 'u'; break;
				case 80: returnValue = 'd'; break;
				}
				break;
			}
		}
	}
	return returnValue;
}

void BingoBoard::remakeBoard(int size) {
	this->size = size;
	//빙고 배열 생성
	board = make_unique<unique_ptr<Bnumber[]>[]>(size);
	for (int i = 0; i < size; i++) {
		board[i] = make_unique<Bnumber[]>(size);
	}
	int k = 1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j].number = k++;
			board[i][j].check = 0;
		}
	}
	//빙고 배열 섞기
	random_device rd;

	mt19937 genNum(rd());

	uniform_int_distribution<int> ranNum(0, size - 1);
	int temp = 0;
	int rdRow;
	int rdCol;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			rdRow = ranNum(genNum);
			rdCol = ranNum(genNum);
			temp = board[rdRow][rdCol].number;
			board[rdRow][rdCol].number = board[i][j].number;
			board[i][j].number = temp;
		}
	}
}

vector<int> BingoBoard::boardtoVector(int size) {
	vector<int> v;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			v.push_back(board[i][j].number);
		}
	}
	return v;
}

void BingoBoard::multiBingoBoardCopy(vector<int> opBoard,int size) {
	int idx = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j].number = opBoard[idx++];
			board[i][j].check = 0;
		}
	}
}

//유저가 번호 선택하는 함수
int BingoBoard::multiChooseNum(int& count)
{
	while (_kbhit()) {
		_getch();  // 남아있는 입력 데이터를 모두 비워줌
	}
	char input = NULL;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	if (y == -1) {
		gotoxy(20, 1);
		cout << "┌────────────┐";
		gotoxy(20, 2);
		cout << "│  나가기    │";
		gotoxy(20, 3);
		cout << "└────────────┘";
	}
	else {
		gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
		cout << "┌───┐";
		gotoxy(4 + x * 4 - 2, 6 + y * 2);
		cout << "│";
		gotoxy(4 + x * 4 + 2, 6 + y * 2);
		cout << "│";
		gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
		cout << "└───┘ ";
	}
	while (input != 'e') {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		if (y == -1) {
			gotoxy(20, 1);
			cout << "┌────────────┐";
			gotoxy(20, 2);
			cout << "│  나가기    │";
			gotoxy(20, 3);
			cout << "└────────────┘";
		}
		else {
			gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
			cout << "      ";
			gotoxy(4 + x * 4 - 2, 6 + y * 2);
			cout << "  ";
			gotoxy(4 + x * 4 + 2, 6 + y * 2);
			cout << "  ";
			gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
			cout << "      ";
		}
		switch (input) {
		case 'l': if (y > -1) x--; break;
		case 'r': if (y > -1) x++; break;
		case 'u':y--; break;
		case 'd':y++; break;
		}
		if (x < 0) {
			x = 0;
		}
		else if (x >= size) {
			x = size - 1;
		}
		if (y < -1) {
			y = -1;
		}
		else if (y >= size) {
			y = size - 1;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		if (y == -1) {
			gotoxy(20, 1);
			cout << "┌────────────┐";
			gotoxy(20, 2);
			cout << "│  나가기    │";
			gotoxy(20, 3);
			cout << "└────────────┘";
		}
		else {
			gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
			cout << "┌───┐";
			gotoxy(4 + x * 4 - 2, 6 + y * 2);
			cout << "│";
			gotoxy(4 + x * 4 + 2, 6 + y * 2);
			cout << "│";
			gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
			cout << "└───┘ ";
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		if (input == 'e') {
			if (board[x][y].check > 0) {
				input = NULL;
				continue;
			}
		}
	}
	gotoxy(4 + x * 4 - 2, 6 + y * 2 - 1);
	cout << "      ";
	gotoxy(4 + x * 4 - 2, 6 + y * 2);
	cout << "  ";
	gotoxy(4 + x * 4 + 2, 6 + y * 2);
	cout << "  ";
	gotoxy(4 + x * 4 - 2, 6 + y * 2 + 1);
	cout << "      ";
	if (y == -1) {
		return -1;
	}
	else {
		board[x][y].check = ++count;
		return board[x][y].number;
	}

}

//빙고판 출력 함수
void BingoBoard::multiPrintBoard(const int& locate)
{
	if (locate == 0) {
		gotoxy(4, 4);
		cout << "User";
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; j++) {
				gotoxy(4 + i * 4, 6 + j * 2);
				if (board[i][j].check > 0) {
					if (board[i][j].check % 2 == 0) {						
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					}
					else {						
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					}
				}
				cout << board[i][j].number;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
	else {
		gotoxy(100 - 12, 4);
		cout << "Opponent";
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				gotoxy(100 - size * 4 + i * 4, 6 + j * 2);
				if (board[i][j].check > 0) {
					if (board[i][j].check % 2 == 0) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					}
				}
				cout << board[i][j].number;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
}