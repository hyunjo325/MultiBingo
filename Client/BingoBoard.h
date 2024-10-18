#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <Windows.h>
#include <conio.h>
#include <fstream>
using namespace std;
class BingoBoard
{
private:
	struct Bnumber
	{
		//number는 숫자, check는 부른 순서
		int number, check;
		Bnumber(int number = 0, int check = 0) noexcept : number(number), check(check) { }

	};
	int size = 0;
	int x = 0, y = 0;
	unique_ptr<unique_ptr<Bnumber[]>[]> board;


public:
	BingoBoard() = default;
	BingoBoard(const int& size);
	~BingoBoard() = default;
	void printBoard(const int& locate);
	int chooseNum(int& count);
	int ComChooseNum(int& count);
	void checkNum(int num, int& count);
	int checkWin();
	void saveBoard(int num, int& count);
	void resumeBoard(int num);
	void replayBoard();
	char inputKeycode();
	void remakeBoard(int size);
	vector<int> boardtoVector(int size);
	void multiBingoBoardCopy(vector<int> opBoard, int size);
	int multiChooseNum(int& count);
	void multiPrintBoard(const int& locate);
	void setCursorView(bool visible) {
		CONSOLE_CURSOR_INFO cursor = { 1, visible };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	}
	void gotoxy(int x, int y) {
		COORD Pos = { (short)x, (short)y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}
};

