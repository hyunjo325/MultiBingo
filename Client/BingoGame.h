#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <thread>
#include <regex>
#include <winsock2.h>  // Windows 소켓 라이브러리
#pragma comment(lib, "ws2_32.lib")  // 소켓 라이브러리 링크
#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#include "BingoBoard.h"
using namespace std;

struct Message {
	int senderID;      // 발신자 ID
	int receiverID;    // 수신자 ID
	std::string content; // 메시지 내용
};
class BingoGame
{
private:
	vector<int> numberRecord;
	unique_ptr<int> numberList;
	int size = 3;
	int count = 0;
	int result = 0;
	int restart = -1;
	BingoBoard user;
	BingoBoard com;
	BingoBoard opponent;
public:
	void play();
	int menu();
	void playGame(BingoBoard& user, BingoBoard& com);
	void newGame();
	int chooseSize();
	void resumeGame();
	void multiGame(BingoBoard& user, BingoBoard& opponent);
	void receiveMessages(SOCKET clientSocket);
	void closeGame();
	char inputKeycode();
	int resultGame(BingoBoard& u, BingoBoard& c);
	int multiResultGame(BingoBoard& user, BingoBoard& opponent);
	int afterGame();
	int afterMultiGame(int restart);
	void setCursorView(bool visible) {
		CONSOLE_CURSOR_INFO cursor = { 1, visible };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	}
	void gotoxy(int x, int y) {
		COORD Pos = { (short)x, (short)y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}
};

