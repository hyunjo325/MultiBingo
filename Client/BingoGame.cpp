#pragma once
#include "BingoGame.h"

//메인 플레이 함수
void BingoGame::play()
{
	system("mode con:cols=100 lines=24");
	gotoxy(38,1);
	cout << "방현조 - 빙고게임" << endl;
	int men = 0;
	while (true) {
		men = menu();
		switch (men) {
		case 0: newGame(); break;
		case 1: resumeGame(); break;
		case 2:  break;
		case 3: closeGame(); return; break;
		}
		if (men == 0) {
			BingoBoard user(size);
			BingoBoard com(size);
			playGame(user, com);
		}
		else if (men == 1) {
			BingoBoard user(size);
			BingoBoard com(size);
			user.resumeBoard(0);
			com.resumeBoard(1);
			playGame(user, com);
		}
		else if (men == 2) {
			BingoBoard user(size);
			BingoBoard opponent(size);
			multiGame(user, opponent);
		}
	}
}

//메뉴 함수
int BingoGame::menu()
{
	system("cls");
	setCursorView(false);
	gotoxy(38, 1);
	cout << "방현조 - 빙고게임" << endl;
	char input = NULL;
	int menuNum = 0;
	
	long length = 0;
	while (input != 'e') {
		gotoxy(10, 3);
		cout << "┌────────────────┐";
		gotoxy(10, 4);
		cout << "│  1) 새 게임    │";
		gotoxy(10, 5);
		cout << "└────────────────┘";
		gotoxy(30, 3);
		cout << "┌────────────────┐";
		gotoxy(30, 4);
		cout << "│  2) 이어하기   │";
		gotoxy(30, 5);
		cout << "└────────────────┘";
		gotoxy(50, 3);
		cout << "┌────────────────┐";
		gotoxy(50, 4);
		cout << "│  3) 멀티플레이 │";
		gotoxy(50, 5);
		cout << "└────────────────┘";
		gotoxy(70, 3);
		cout << "┌────────────────┐";
		gotoxy(70, 4);
		cout << "│  4) 종료       │";
		gotoxy(70, 5);
		cout << "└────────────────┘";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(10, 3);
			cout << "┌────────────────┐";
			gotoxy(10, 4);
			cout << "│  1) 새 게임    │";
			gotoxy(10, 5);
			cout << "└────────────────┘";
			break;
		case 1:
			if (length == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			}
			gotoxy(30, 3);
			cout << "┌────────────────┐";
			gotoxy(30, 4);
			cout << "│  2) 이어하기   │";
			gotoxy(30, 5);
			cout << "└────────────────┘";
			break;
		case 2:
			gotoxy(50, 3);
			cout << "┌────────────────┐";
			gotoxy(50, 4);
			cout << "│  3) 멀티플레이 │";
			gotoxy(50, 5);
			cout << "└────────────────┘";
			break;
		case 3:
			gotoxy(70, 3);
			cout << "┌────────────────┐";
			gotoxy(70, 4);
			cout << "│  4) 종료       │";
			gotoxy(70, 5);
			cout << "└────────────────┘";
			break;
		default: break;
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		switch (input) {
		case 'l': menuNum--; break;
		case 'r': menuNum++; break;
		case 'e': break;
		default: break;
		}
		if (menuNum < 0) {
			menuNum = 0;
		}
		if (menuNum > 3) {
			menuNum = 3;
		}
		const char* file = "./userdata.dat";
		ifstream fin(file, ios::in | ios::out | ios::binary);
		if (!fin) {
			ofstream fout(file, ios::out | ios::binary);
			if (!fout) {
				cout << "파일 생성 오류" << endl;
				return 1; // 오류 코드 반환
			}

			fout.close();
		}
		fin.seekg(0, ios::end);
		length = (long)fin.tellg();
		if (length == 0 && menuNum == 1) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			gotoxy(30, 3);
			cout << "┌────────────────┐";
			gotoxy(30, 4);
			cout << "│  2) 이어하기   │";
			gotoxy(30, 5);
			cout << "└────────────────┘";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			input = NULL;
			continue;
		}
	}
	
	return menuNum;
}

//빙고게임을 진행하는 함수
void BingoGame::playGame(BingoBoard& user, BingoBoard& com)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("cls");
	gotoxy(38, 2);
	cout << "방현조 - 빙고게임" << endl;
	gotoxy(20, 1);
	cout << "┌────────────┐";
	gotoxy(20, 2);
	cout << "│  저장하기  │";
	gotoxy(20, 3);
	cout << "└────────────┘";
	user.printBoard(0);
	//com.printBoard(1);
	while (true) {
		int mynum = user.chooseNum(count);
		if (mynum == -1) {
			user.saveBoard(0, count);
			com.saveBoard(1, count);
			gotoxy(38, 3);
			cout << "빙고게임이 저장되었습니다." << endl;
			gotoxy(36, 4);
			cout << "자동으로 메인메뉴로 이동합니다." << endl;
			Sleep(1000);
			return;
		}
		com.checkNum(mynum, count);
		user.printBoard(0);
		//com.printBoard(1);
		result = resultGame(user, com);
		if (result != 0)
			break;
		Sleep(300);
		int comnum = com.ComChooseNum(count);
		user.checkNum(comnum, count);
		user.printBoard(0);
		//com.printBoard(1);
		result = resultGame(user, com);
		if (result != 0)
			break;
	}
	com.printBoard(1);
	while (true) {
		if (afterGame()) {
			user.replayBoard();
		}
		else
			break;
	}
}

//새 게임을 시작하기 위해 초기화하는 함수
void BingoGame::newGame()
{	
	const char* file = "./userdata.dat";
	const char* file2 = "./comdata.dat";
	ifstream fin(file, ios::out | ios::trunc);
	fin.close();
	ifstream fin2(file2, ios::out | ios::trunc);
	fin2.close();
	size = chooseSize();
	count = 0;
	return;
}

//빙고판 크기를 정하는 함수
int BingoGame::chooseSize()
{
	char input = NULL;
	int size = 3;
	system("cls");
	gotoxy(38, 1);
	cout << "방현조 - 빙고게임" << endl;
	gotoxy(38, 3);
	cout << "빙고판의 크기를 정해 주세요";
	while (input != 'e') {
		for (int i = 0; i < 7; i++) {
			gotoxy(22 + i * 8, 5);
			cout << "┌──────┐";
			gotoxy(22 + i * 8, 6);
			cout << "│ " << i + 3 << "×" << i + 3 << "  │";
			gotoxy(22 + i * 8, 7);
			cout << "└──────┘";
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		gotoxy(22 + (size - 3) * 8, 5);
		cout << "┌──────┐";
		gotoxy(22 + (size - 3) * 8, 6);
		cout << "│ " << size << "×" << size << "  │";
		gotoxy(22 + (size - 3) * 8, 7);
		cout << "└──────┘";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		switch (input) {
		case 'l': size--; break;
		case 'r': size++; break;
		case 'e': break;
		default: break;
		}
		if (size < 3) {
			size = 3;
		}
		if (size > 9) {
			size = 9;
		}

	}
	return size;
}

//이어하기 위해 크기와 순서를 불러오는 함수
void BingoGame::resumeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "방현조 - 빙고게임" << endl;
	gotoxy(38, 2);
	cout << "빙고게임이 다시 시작되었습니다." << endl;
	const char* file = "./userdata.dat";
	ifstream fin(file, ios::in | ios::binary);
	if (!fin) {
		cout << "유저파일 열기 오류";
		return;
	}
	fin.read((char*)(&count), sizeof(count));
	fin.read((char*)(&size), sizeof(size));
	fin.close();
	
	return;
}

//프로그램을 종료하는 함수
void BingoGame::closeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "방현조 - 빙고게임" << endl;
	gotoxy(38, 2);
	cout << "빙고게임이 종료되었습니다." << endl;
	
}

//키보드 입력을 받는 함수
char BingoGame::inputKeycode()
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

//게임 결과 확인하는 함수
int BingoGame::resultGame(BingoBoard& u, BingoBoard& c)
{
	if (u.checkWin() == -1 && c.checkWin() == -1) {
		gotoxy(42, 3);
		cout << "무승부, 진행불가 ";
		return -2;
	}
	else if (u.checkWin() > c.checkWin()) {
		gotoxy(42, 3);
		cout << "사용자의 승리    ";
		return 1;
	}
	else if (u.checkWin() < c.checkWin()) {
		gotoxy(42, 3);
		cout << "컴퓨터의 승리    ";
		return -1;
	}
	else if (u.checkWin() > 0 && c.checkWin() > 0 && (u.checkWin() == c.checkWin())) {
		gotoxy(42, 3);
		cout << "무승부, 계속 진행";
		return 0;
	}
	else
		return 0;
}

//게임이 끝나고 파일 초기화 하고 리플레이하거나 메인화면 가는 함수
int BingoGame::afterGame()
{
	const char* file = "./userdata.dat";
	const char* file2 = "./comdata.dat";
	ifstream fin(file, ios::out | ios::trunc);
	fin.close();
	ifstream fin2(file2, ios::out | ios::trunc);
	fin2.close();
	int menuNum = 0;
	char input = NULL;
	gotoxy(20, 1);
	cout << "              ";
	gotoxy(20, 2);
	cout << "              ";
	gotoxy(20, 3);
	cout << "              ";

	while (input != 'e') {

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(42, 6);
		cout << "┌────────────┐";
		gotoxy(42, 7);
		cout << "│ 메인메뉴로 │";
		gotoxy(42, 8);
		cout << "└────────────┘";

		gotoxy(42, 10);
		cout << "┌────────────┐";
		gotoxy(42, 11);
		cout << "│  리플레이  │";
		gotoxy(42, 12);
		cout << "└────────────┘";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "┌────────────┐";
			gotoxy(42, 7);
			cout << "│ 메인메뉴로 │";
			gotoxy(42, 8);
			cout << "└────────────┘";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "┌────────────┐";
			gotoxy(42, 11);
			cout << "│  리플레이  │";
			gotoxy(42, 12);
			cout << "└────────────┘";
			break;
		
		default: break;
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		switch (input) {
		case 'u': menuNum--; break;
		case 'd': menuNum++; break;
		case 'e': break;
		default: break;
		}
		if (menuNum < 0) {
			menuNum = 0;
		}
		if (menuNum > 1) {
			menuNum = 1;
		}

	}
	return menuNum;
}

mutex stopFlagMutex;
condition_variable cv;
bool stopFlag = false;
string receivedMessage;
mutex messageMutex;

// 서버로부터 메시지를 수신하는 함수
void BingoGame::receiveMessages(SOCKET clientSocket) {
	char buffer[BUFFER_SIZE];
	receivedMessage.clear();
	while (!stopFlag) {
		int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

		if (bytesReceived <= 0) {
			gotoxy(38, 3);
			cerr << "서버와의 연결이 종료되었습니다.\n";
			Sleep(1000);
			stopFlag = true;
			break;
		}

		// 받은 메시지에 따른 동작 처리
		{
			lock_guard<mutex> lock(messageMutex);
			if (bytesReceived > 0) {
				buffer[bytesReceived] = '\0';  // 수신한 데이터 끝에 NULL 문자 추가
				receivedMessage = string(buffer);
				if (receivedMessage == "notice#상대가 게임을 나갔습니다.") {
					gotoxy(38, 2);
					cout << "상대가 게임을 나갔습니다." << endl;
					Sleep(1000);
					stopFlag = true; 
					closesocket(clientSocket);  // 소켓 닫기
					WSACleanup();
					break;
				}
				vector<string> parts;
				istringstream ss(receivedMessage);
				string item;
				// '#' 기준으로 분할
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}
				// 파싱 결과 출력
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "thread부분 " << i + 1 << ": " << parts[i] << "\n";
				}
				gotoxy(0, 0);
				//cout << receivedMessage;
			}
		}
	}
	closesocket(clientSocket);  // 소켓 닫기
	WSACleanup();  // Winsock 종료
}
void BingoGame::multiGame(BingoBoard& user, BingoBoard& opponent) {
	system("cls");
	gotoxy(0, 0);

	size = 3;
	result = 0;
	count = 0;
	restart = -1;
	stopFlag = false;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);  // WinSock 초기화

	char buffer[BUFFER_SIZE];
	SOCKET clientSocket;
	struct sockaddr_in serverAddr;
	int myClientID = -1;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "소켓 생성 실패\n";
		return;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	// inet_addr를 사용하여 IP 주소를 변환
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // 여기서 IP 주소를 설정합니다.

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "연결 실패\n";
		return;
	}

	char input = NULL;
	int roomid = 0;
	stopFlag = false;
	cout << "서버에 연결되었습니다.\n";
	int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // 수신한 데이터 끝에 NULL 문자 추가
		myClientID = stoi(string(buffer));
	}
	cout << "클라이언트 ID : " << myClientID << "\n";
	receivedMessage.clear();
	thread receiveThread(&BingoGame::receiveMessages, this, clientSocket);
	receiveThread.detach();
	string nowStatus = "NULL";
	vector<string> parts;
	mutex partsMutex;
	while (!stopFlag) {
		{
			lock_guard<mutex> lock(messageMutex);
			if (!receivedMessage.empty()) {
				nowStatus = receivedMessage;
				parts.clear();
				istringstream ss(nowStatus);
				string item;

				// '#' 기준으로 분할
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}

				// 파싱 결과 출력
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "while부분 " << i + 1 << ": " << parts[i] << "\n";
				}
				nowStatus = parts[0];
				receivedMessage.clear();  // 메시지 처리 후 비움
			}
		}
		if (nowStatus == "new") {
			size = 3;
			count = 0;
			result = 0;
			restart = -1;
			input = NULL;
			if (parts[1] == "1") {
				system("cls");
				gotoxy(0, 0);
				cout << "다른 참가자를 기다리는중..\n";
				cout << "빙고판의 크기를 정해 주세요";
				while (input != 'e') {
					for (int i = 0; i < 7; i++) {
						gotoxy(22 + i * 8, 5);
						cout << "┌──────┐";
						gotoxy(22 + i * 8, 6);
						cout << "│ " << i + 3 << "×" << i + 3 << "  │";
						gotoxy(22 + i * 8, 7);
						cout << "└──────┘";
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

					gotoxy(22 + (size - 3) * 8, 5);
					cout << "┌──────┐";
					gotoxy(22 + (size - 3) * 8, 6);
					cout << "│ " << size << "×" << size << "  │";
					gotoxy(22 + (size - 3) * 8, 7);
					cout << "└──────┘";

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					input = inputKeycode();
					switch (input) {
					case 'l': size--; break;
					case 'r': size++; break;
					case 'e': break;
					default: break;
					}
					if (size < 3) {
						size = 3;
					}
					if (size > 9) {
						size = 9;
					}

				}
				string sizemsg = "size#";
				sizemsg += to_string(roomid);
				sizemsg += "#";
				sizemsg += to_string(size);
				send(clientSocket, sizemsg.c_str(), sizemsg.length(), 0);
				system("cls");
				gotoxy(0, 0);
				cout << "빙고판 크기 : " << size << "설정 완료\n";
				cout << "다른 참가자를 기다리는중..\n";
				nowStatus = "NULL";
			}
			else {
				gotoxy(0, 0);
				cout << "먼저 다시하기를 누른 상대방이 빙고판 크기를 정하고 있습니다.";
			}
		}
		else if (nowStatus == "start") {
			system("cls");
			gotoxy(0, 0);
			count = 0;
			result = 0;
			restart = -1;
			roomid = stoi(parts[1]);
			size = stoi(parts[2]);
			cout << "빙고판의 크기를 " << size << "×" << size << "로 게임을 시작합니다.\n";
			user.remakeBoard(size);
			vector<int> v = user.boardtoVector(size);
			string userBoradInfo = "board#";
			userBoradInfo += to_string(roomid);
			userBoradInfo += "#";
			userBoradInfo += to_string(myClientID);
			for (int i = 0; i < v.size(); i++) {
				userBoradInfo += "#";
				userBoradInfo += to_string(v[i]);
			}
			/*gotoxy(38,2);
			cout << userBoradInfo << endl;*/
			send(clientSocket, userBoradInfo.c_str(), userBoradInfo.length(), 0);

			nowStatus = "NULL";
		}
		else if (nowStatus == "board") {
			vector<int> opboard;
			lock_guard<mutex> lock(partsMutex);
			for (int i = 3; i < parts.size(); i++) {
				opboard.push_back(stoi(parts[i]));
			}
			opponent.remakeBoard(size);
			opponent.multiBingoBoardCopy(opboard, size);
			//cout << "상대 보드 저장 완료\n";
			string setmsg = "done#";
			setmsg += to_string(roomid);
			setmsg += "#";
			setmsg += to_string(myClientID);
			send(clientSocket, setmsg.c_str(), setmsg.length(), 0);
			nowStatus = "NULL";
		}
		else if (nowStatus == "set") {
			gotoxy(38, 2);
			cout << "잠시 후 멀티 빙고 게임이 시작됩니다" << endl;
			nowStatus = "NULL";
			Sleep(1000);
			system("cls");
		}
		else if (nowStatus == "notice") {
			nowStatus = "NULL";
			gotoxy(36, 4);
			cout << "자동으로 메인메뉴로 이동합니다?" << endl;
			Sleep(1000);
			size = 0;
			result = 0;
			restart = -1;
			stopFlag = false;
			WSACleanup();  // WinSock 종료
			return;
		}
		else if (nowStatus == "turn") {
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gotoxy(38, 2);
			cout << "방현조 - 빙고게임" << endl;
			gotoxy(20, 1);
			cout << "┌────────────┐";
			gotoxy(20, 2);
			cout << "│  나가기    │";
			gotoxy(20, 3);
			cout << "└────────────┘";
			gotoxy(38, 3);
			user.multiPrintBoard(0);
			//opponent.multiPrintBoard(1);
			while (!stopFlag) {
				{
					lock_guard<mutex> lock(messageMutex);
					if (!receivedMessage.empty()) {
						parts.clear();
						istringstream ss(receivedMessage);
						string item;
						// '#' 기준으로 분할
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}
						// 파싱 결과 출력
						for (size_t i = 0; i < parts.size(); ++i) {
							//cout << "부분 " << i + 1 << ": " << parts[i] << "\n";
						}
						nowStatus = parts[0];
						receivedMessage.clear();  // 메시지 처리 후 비움
						//system("cls");
						gotoxy(38, 2);
						cout << "방현조 - 빙고게임" << endl;
						gotoxy(20, 1);
						cout << "┌────────────┐";
						gotoxy(20, 2);
						cout << "│  나가기    │";
						gotoxy(20, 3);
						cout << "└────────────┘";
						gotoxy(38, 3);
						user.multiPrintBoard(0);
						//opponent.multiPrintBoard(1);
					}
				}
				if (result==0 && nowStatus == "turn" && stoi(parts[2]) == myClientID) {
					gotoxy(38, 3);
					cout << "당신 차례입니다." << endl;
					int mynum = user.multiChooseNum(count);
					if (mynum == -1) {
						gotoxy(38, 3);
						cout << "멀티 빙고 게임을 중단합니다." << endl;
						gotoxy(36, 4);
						cout << "1초 후 메인메뉴로 이동합니다." << endl;
						Sleep(1000);
						stopFlag = true;
						parts.clear();
						nowStatus = "NULL"; 
						shutdown(clientSocket, SD_BOTH);
						system("cls");
						break;
					}
					//system("cls");
					user.multiPrintBoard(0);
					//opponent.multiPrintBoard(1);
					string userBoradInfo = "num#";
					userBoradInfo += to_string(roomid);
					userBoradInfo += "#";
					userBoradInfo += to_string(mynum);
					send(clientSocket, userBoradInfo.c_str(), userBoradInfo.length(), 0);
				}
				else if (result == 0 && nowStatus == "turn" && stoi(parts[2]) != myClientID) {
					gotoxy(38, 3);
					cout << "상대 차례입니다." << endl;
				}
				else if (nowStatus == "num") {
					int nownum = stoi(parts[2]);
					int nowcount = stoi(parts[3]);
					user.checkNum(nownum, nowcount);
					opponent.checkNum(nownum, nowcount);
					count = nowcount;
					user.multiPrintBoard(0);
					//opponent.multiPrintBoard(1);
					result = multiResultGame(user, opponent);
					if (result != 0) {
						string resultInfo = "end#";
						resultInfo += to_string(roomid);
						resultInfo += "#";
						resultInfo += to_string(myClientID);
						resultInfo += "#";
						resultInfo += to_string(result);
						send(clientSocket, resultInfo.c_str(), resultInfo.length(), 0);
						nowStatus = "NULL";
						break;
					}
					else {
						string resultInfo = "notend#";
						resultInfo += to_string(roomid);
						resultInfo += "#";
						resultInfo += to_string(myClientID);
						resultInfo += "#";
						resultInfo += to_string(result);
						send(clientSocket, resultInfo.c_str(), resultInfo.length(), 0);
						nowStatus = "NULL";
					}
				}
				nowStatus = "NULL";
			}
			int re = -1;
			opponent.multiPrintBoard(1);
			restart = afterMultiGame(re);
			while (!stopFlag) {
				{
					lock_guard<mutex> lock(messageMutex);
					if (!receivedMessage.empty()) {
						vector<string> parts;
						istringstream ss(receivedMessage);
						string item;
						// '#' 기준으로 분할
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}

						if (parts[0] == "new" || parts[0] == "start") {
							gotoxy(32, 3);
							cout << "수락 완료! 3초 후 다음 게임을 진행합니다";
							Sleep(3000);
							system("cls");
							break;
						}
					}
				}
				if (re == -1 || (re==2&& restart==0)) {
					string restartInfo = "restart#";
					restartInfo += to_string(roomid);
					restartInfo += "#";
					restartInfo += to_string(myClientID);
					restartInfo += "#";
					restartInfo += to_string(restart);
					send(clientSocket, restartInfo.c_str(), restartInfo.length(), 0);
				}if (restart == -2) {
					restart = -1;
					stopFlag = false;
					break;
				}
				else if (restart == 1) {
					re = 2;
					gotoxy(38, 2);
					cout << "상대방 수락을 기다리는 중" << endl;
				}
				else if(restart == 0){
					stopFlag = true;
					parts.clear();
					nowStatus = "NULL";
					system("cls");
					break;
				}
			}
		}
		else {
			//system("cls");
			//gotoxy(40, 10);
			//cout << nowStatus << "\n";
		}
	}
	system("cls");
	{
		std::lock_guard<std::mutex> lock(stopFlagMutex);
		stopFlag = true;
		//shutdown(clientSocket, SD_BOTH);
	}

	result = 0;
	restart = -1;
	stopFlag = false;
	WSACleanup();  // WinSock 종료
	return;
} 

//게임 결과 확인하는 함수
int BingoGame::multiResultGame(BingoBoard& user, BingoBoard& opponent)
{
	if (user.checkWin() == -1 && opponent.checkWin() == -1) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "무승부, 진행불가 ";		
		return -2;
	}
	else if (user.checkWin() > opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "사용자의 승리    ";
		return 1;
	}
	else if (user.checkWin() < opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "상대방의 승리    ";
		return -1;
	}
	else if (user.checkWin() > 0 && opponent.checkWin() > 0 && (user.checkWin() == opponent.checkWin())) {
		gotoxy(42, 4);
		cout << "무승부, 계속 진행";
		return 0;
	}
	else
		return 0;
}

//게임이 끝나고 파일 초기화 하고 리플레이하거나 메인화면 가는 함수
int BingoGame::afterMultiGame(int restart)
{
	int menuNum = 0;
	char input = NULL;
	gotoxy(20, 1);
	cout << "              ";
	gotoxy(20, 2);
	cout << "              ";
	gotoxy(20, 3);
	cout << "              ";

	while (input != 'e'&& !stopFlag) {
		input = NULL;
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(42, 6);
		cout << "┌────────────┐";
		gotoxy(42, 7);
		cout << "│ 메인메뉴로 │";
		gotoxy(42, 8);
		cout << "└────────────┘";

		gotoxy(42, 10);
		cout << "┌────────────┐";
		gotoxy(42, 11);
		cout << "│  다시하기  │";
		gotoxy(42, 12);
		cout << "└────────────┘";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "┌────────────┐";
			gotoxy(42, 7);
			cout << "│ 메인메뉴로 │";
			gotoxy(42, 8);
			cout << "└────────────┘";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "┌────────────┐";
			gotoxy(42, 11);
			cout << "│  다시하기  │";
			gotoxy(42, 12);
			cout << "└────────────┘";
			break;

		default: break;
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		input = inputKeycode();
		switch (input) {
		case 'u': menuNum--; break;
		case 'd': menuNum++; break;
		case 'e': break;
		default: break;
		}
		if (menuNum < 0) {
			menuNum = 0;
		}
		if (menuNum > 1) {
			menuNum = 1;
		}

	}
	return menuNum;
}