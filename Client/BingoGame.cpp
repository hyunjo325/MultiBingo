#pragma once
#include "BingoGame.h"

//詭檣 Ы溯檜 л熱
void BingoGame::play()
{
	system("mode con:cols=100 lines=24");
	gotoxy(38,1);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
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

//詭景 л熱
int BingoGame::menu()
{
	system("cls");
	setCursorView(false);
	gotoxy(38, 1);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
	char input = NULL;
	int menuNum = 0;
	
	long length = 0;
	while (input != 'e') {
		gotoxy(10, 3);
		cout << "忙式式式式式式式式式式式式式式式式忖";
		gotoxy(10, 4);
		cout << "弛  1) 億 啪歜    弛";
		gotoxy(10, 5);
		cout << "戌式式式式式式式式式式式式式式式式戎";
		gotoxy(30, 3);
		cout << "忙式式式式式式式式式式式式式式式式忖";
		gotoxy(30, 4);
		cout << "弛  2) 檜橫ж晦   弛";
		gotoxy(30, 5);
		cout << "戌式式式式式式式式式式式式式式式式戎";
		gotoxy(50, 3);
		cout << "忙式式式式式式式式式式式式式式式式忖";
		gotoxy(50, 4);
		cout << "弛  3) 詩じЫ溯檜 弛";
		gotoxy(50, 5);
		cout << "戌式式式式式式式式式式式式式式式式戎";
		gotoxy(70, 3);
		cout << "忙式式式式式式式式式式式式式式式式忖";
		gotoxy(70, 4);
		cout << "弛  4) 謙猿       弛";
		gotoxy(70, 5);
		cout << "戌式式式式式式式式式式式式式式式式戎";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(10, 3);
			cout << "忙式式式式式式式式式式式式式式式式忖";
			gotoxy(10, 4);
			cout << "弛  1) 億 啪歜    弛";
			gotoxy(10, 5);
			cout << "戌式式式式式式式式式式式式式式式式戎";
			break;
		case 1:
			if (length == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			}
			gotoxy(30, 3);
			cout << "忙式式式式式式式式式式式式式式式式忖";
			gotoxy(30, 4);
			cout << "弛  2) 檜橫ж晦   弛";
			gotoxy(30, 5);
			cout << "戌式式式式式式式式式式式式式式式式戎";
			break;
		case 2:
			gotoxy(50, 3);
			cout << "忙式式式式式式式式式式式式式式式式忖";
			gotoxy(50, 4);
			cout << "弛  3) 詩じЫ溯檜 弛";
			gotoxy(50, 5);
			cout << "戌式式式式式式式式式式式式式式式式戎";
			break;
		case 3:
			gotoxy(70, 3);
			cout << "忙式式式式式式式式式式式式式式式式忖";
			gotoxy(70, 4);
			cout << "弛  4) 謙猿       弛";
			gotoxy(70, 5);
			cout << "戌式式式式式式式式式式式式式式式式戎";
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
				cout << "だ橾 儅撩 螃盟" << endl;
				return 1; // 螃盟 囀萄 奩��
			}

			fout.close();
		}
		fin.seekg(0, ios::end);
		length = (long)fin.tellg();
		if (length == 0 && menuNum == 1) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			gotoxy(30, 3);
			cout << "忙式式式式式式式式式式式式式式式式忖";
			gotoxy(30, 4);
			cout << "弛  2) 檜橫ж晦   弛";
			gotoxy(30, 5);
			cout << "戌式式式式式式式式式式式式式式式式戎";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			input = NULL;
			continue;
		}
	}
	
	return menuNum;
}

//綵堅啪歜擊 霞чж朝 л熱
void BingoGame::playGame(BingoBoard& user, BingoBoard& com)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("cls");
	gotoxy(38, 2);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
	gotoxy(20, 1);
	cout << "忙式式式式式式式式式式式式忖";
	gotoxy(20, 2);
	cout << "弛  盪濰ж晦  弛";
	gotoxy(20, 3);
	cout << "戌式式式式式式式式式式式式戎";
	user.printBoard(0);
	//com.printBoard(1);
	while (true) {
		int mynum = user.chooseNum(count);
		if (mynum == -1) {
			user.saveBoard(0, count);
			com.saveBoard(1, count);
			gotoxy(38, 3);
			cout << "綵堅啪歜檜 盪濰腎歷蝗棲棻." << endl;
			gotoxy(36, 4);
			cout << "濠翕戲煎 詭檣詭景煎 檜翕м棲棻." << endl;
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

//億 啪歜擊 衛濛ж晦 嬪п 蟾晦�倆炴� л熱
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

//綵堅っ 觼晦蒂 薑ж朝 л熱
int BingoGame::chooseSize()
{
	char input = NULL;
	int size = 3;
	system("cls");
	gotoxy(38, 1);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
	gotoxy(38, 3);
	cout << "綵堅っ曖 觼晦蒂 薑п 輿撮蹂";
	while (input != 'e') {
		for (int i = 0; i < 7; i++) {
			gotoxy(22 + i * 8, 5);
			cout << "忙式式式式式式忖";
			gotoxy(22 + i * 8, 6);
			cout << "弛 " << i + 3 << "▼" << i + 3 << "  弛";
			gotoxy(22 + i * 8, 7);
			cout << "戌式式式式式式戎";
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		gotoxy(22 + (size - 3) * 8, 5);
		cout << "忙式式式式式式忖";
		gotoxy(22 + (size - 3) * 8, 6);
		cout << "弛 " << size << "▼" << size << "  弛";
		gotoxy(22 + (size - 3) * 8, 7);
		cout << "戌式式式式式式戎";

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

//檜橫ж晦 嬪п 觼晦諦 牖憮蒂 碳楝螃朝 л熱
void BingoGame::resumeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
	gotoxy(38, 2);
	cout << "綵堅啪歜檜 棻衛 衛濛腎歷蝗棲棻." << endl;
	const char* file = "./userdata.dat";
	ifstream fin(file, ios::in | ios::binary);
	if (!fin) {
		cout << "嶸盪だ橾 翮晦 螃盟";
		return;
	}
	fin.read((char*)(&count), sizeof(count));
	fin.read((char*)(&size), sizeof(size));
	fin.close();
	
	return;
}

//Щ煎斜極擊 謙猿ж朝 л熱
void BingoGame::closeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "寞⑷褻 - 綵堅啪歜" << endl;
	gotoxy(38, 2);
	cout << "綵堅啪歜檜 謙猿腎歷蝗棲棻." << endl;
	
}

//酈爾萄 殮溘擊 嫡朝 л熱
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

//啪歜 唸婁 �挫恉炴� л熱
int BingoGame::resultGame(BingoBoard& u, BingoBoard& c)
{
	if (u.checkWin() == -1 && c.checkWin() == -1) {
		gotoxy(42, 3);
		cout << "鼠蝓睡, 霞ч碳陛 ";
		return -2;
	}
	else if (u.checkWin() > c.checkWin()) {
		gotoxy(42, 3);
		cout << "餌辨濠曖 蝓葬    ";
		return 1;
	}
	else if (u.checkWin() < c.checkWin()) {
		gotoxy(42, 3);
		cout << "闡У攪曖 蝓葬    ";
		return -1;
	}
	else if (u.checkWin() > 0 && c.checkWin() > 0 && (u.checkWin() == c.checkWin())) {
		gotoxy(42, 3);
		cout << "鼠蝓睡, 啗樓 霞ч";
		return 0;
	}
	else
		return 0;
}

//啪歜檜 部釭堅 だ橾 蟾晦�� ж堅 葬Ы溯檜ж剪釭 詭檣�飛� 陛朝 л熱
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
		cout << "忙式式式式式式式式式式式式忖";
		gotoxy(42, 7);
		cout << "弛 詭檣詭景煎 弛";
		gotoxy(42, 8);
		cout << "戌式式式式式式式式式式式式戎";

		gotoxy(42, 10);
		cout << "忙式式式式式式式式式式式式忖";
		gotoxy(42, 11);
		cout << "弛  葬Ы溯檜  弛";
		gotoxy(42, 12);
		cout << "戌式式式式式式式式式式式式戎";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "忙式式式式式式式式式式式式忖";
			gotoxy(42, 7);
			cout << "弛 詭檣詭景煎 弛";
			gotoxy(42, 8);
			cout << "戌式式式式式式式式式式式式戎";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "忙式式式式式式式式式式式式忖";
			gotoxy(42, 11);
			cout << "弛  葬Ы溯檜  弛";
			gotoxy(42, 12);
			cout << "戌式式式式式式式式式式式式戎";
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

// 憮幗煎睡攪 詭衛雖蒂 熱褐ж朝 л熱
void BingoGame::receiveMessages(SOCKET clientSocket) {
	char buffer[BUFFER_SIZE];
	receivedMessage.clear();
	while (!stopFlag) {
		int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

		if (bytesReceived <= 0) {
			gotoxy(38, 3);
			cerr << "憮幗諦曖 翱唸檜 謙猿腎歷蝗棲棻.\n";
			Sleep(1000);
			stopFlag = true;
			break;
		}

		// 嫡擎 詭衛雖縑 評艇 翕濛 籀葬
		{
			lock_guard<mutex> lock(messageMutex);
			if (bytesReceived > 0) {
				buffer[bytesReceived] = '\0';  // 熱褐и 等檜攪 部縑 NULL 僥濠 蹺陛
				receivedMessage = string(buffer);
				if (receivedMessage == "notice#鼻渠陛 啪歜擊 釭鬲蝗棲棻.") {
					gotoxy(38, 2);
					cout << "鼻渠陛 啪歜擊 釭鬲蝗棲棻." << endl;
					Sleep(1000);
					stopFlag = true; 
					closesocket(clientSocket);  // 模鰍 殘晦
					WSACleanup();
					break;
				}
				vector<string> parts;
				istringstream ss(receivedMessage);
				string item;
				// '#' 晦遽戲煎 碟й
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}
				// だ諒 唸婁 轎溘
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "thread睡碟 " << i + 1 << ": " << parts[i] << "\n";
				}
				gotoxy(0, 0);
				//cout << receivedMessage;
			}
		}
	}
	closesocket(clientSocket);  // 模鰍 殘晦
	WSACleanup();  // Winsock 謙猿
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
	WSAStartup(MAKEWORD(2, 2), &wsaData);  // WinSock 蟾晦��

	char buffer[BUFFER_SIZE];
	SOCKET clientSocket;
	struct sockaddr_in serverAddr;
	int myClientID = -1;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "模鰍 儅撩 褒ぬ\n";
		return;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	// inet_addr蒂 餌辨ж罹 IP 輿模蒂 滲��
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // 罹晦憮 IP 輿模蒂 撲薑м棲棻.

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "翱唸 褒ぬ\n";
		return;
	}

	char input = NULL;
	int roomid = 0;
	stopFlag = false;
	cout << "憮幗縑 翱唸腎歷蝗棲棻.\n";
	int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // 熱褐и 等檜攪 部縑 NULL 僥濠 蹺陛
		myClientID = stoi(string(buffer));
	}
	cout << "贗塭檜樹お ID : " << myClientID << "\n";
	string nicknameMsg;
	string nickname;
	regex nicknamePattern("^[a-zA-Z0-9]+$");
	while (true) {
		cout << "棣啻歜擊 殮溘ж撮蹂 (3~12濠 憲だ漯/璋濠): ";
		cin >> nickname;
		if (nickname.length() < 3 || nickname.length() > 12 || !regex_match(nickname, nicknamePattern)) {
			cout << "棣啻歜擎 3~12濠曖 憲だ漯/璋濠虜 陛棟м棲棻.\n";
			continue;
		}
		nicknameMsg = "nickname#" + nickname;
		send(clientSocket, nicknameMsg.c_str(), nicknameMsg.length(), 0);

		int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytesReceived <= 0) {
			cerr << "憮幗 擬港 熱褐 褒ぬ\n";
			return;
		}

		buffer[bytesReceived] = '\0';
		string response(buffer);

		if (response.rfind("login#", 0) == 0 || response.rfind("register#", 0) == 0) {
			vector<string> parts;
			istringstream ss(response);
			string token;
			while (getline(ss, token, '#')) parts.push_back(token);

			string mode = parts[0];
			string nickname = parts[1];
			int win = stoi(parts[2]);
			int lose = stoi(parts[3]);

			cout << "[" << (mode == "login" ? "煎斜檣" : "�蛾灠㊣�") << "] " << nickname << "椒 �紊腎桭炴�.\n";
			cout << "⑷營 瞪瞳 - 蝓: " << win << ", ぬ: " << lose << "\n";
			break;
		}
		else if (response.rfind("error#", 0) == 0) {
			cout << "憮幗 擬港: " << response.substr(6) << "\n";
			continue;
		}
		else {
			cout << "憲 熱 橈朝 憮幗 擬港: " << response << "\n";
			continue;
		}
	}

	Sleep(1000);
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

				// '#' 晦遽戲煎 碟й
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}

				// だ諒 唸婁 轎溘
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "while睡碟 " << i + 1 << ": " << parts[i] << "\n";
				}
				nowStatus = parts[0];
				receivedMessage.clear();  // 詭衛雖 籀葬 �� 綠遺
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
				cout << "棻艇 霤陛濠蒂 晦棻葬朝醞..\n";
				cout << "綵堅っ曖 觼晦蒂 薑п 輿撮蹂";
				while (input != 'e') {
					for (int i = 0; i < 7; i++) {
						gotoxy(22 + i * 8, 5);
						cout << "忙式式式式式式忖";
						gotoxy(22 + i * 8, 6);
						cout << "弛 " << i + 3 << "▼" << i + 3 << "  弛";
						gotoxy(22 + i * 8, 7);
						cout << "戌式式式式式式戎";
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

					gotoxy(22 + (size - 3) * 8, 5);
					cout << "忙式式式式式式忖";
					gotoxy(22 + (size - 3) * 8, 6);
					cout << "弛 " << size << "▼" << size << "  弛";
					gotoxy(22 + (size - 3) * 8, 7);
					cout << "戌式式式式式式戎";

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
				cout << "綵堅っ 觼晦 : " << size << "撲薑 諫猿\n";
				cout << "棻艇 霤陛濠蒂 晦棻葬朝醞..\n";
				nowStatus = "NULL";
			}
			else {
				gotoxy(0, 0);
				cout << "試盪 棻衛ж晦蒂 援艇 鼻渠寞檜 綵堅っ 觼晦蒂 薑ж堅 氈蝗棲棻.";
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
			cout << "綵堅っ曖 觼晦蒂 " << size << "▼" << size << "煎 啪歜擊 衛濛м棲棻.\n";
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
			//cout << "鼻渠 爾萄 盪濰 諫猿\n";
			string setmsg = "done#";
			setmsg += to_string(roomid);
			setmsg += "#";
			setmsg += to_string(myClientID);
			send(clientSocket, setmsg.c_str(), setmsg.length(), 0);
			nowStatus = "NULL";
		}
		else if (nowStatus == "set") {
			gotoxy(38, 2);
			cout << "濡衛 �� 詩じ 綵堅 啪歜檜 衛濛腌棲棻" << endl;
			nowStatus = "NULL";
			Sleep(1000);
			system("cls");
		}
		else if (nowStatus == "notice") {
			nowStatus = "NULL";
			gotoxy(36, 4);
			cout << "濠翕戲煎 詭檣詭景煎 檜翕м棲棻?" << endl;
			Sleep(1000);
			size = 0;
			result = 0;
			restart = -1;
			stopFlag = false;
			WSACleanup();  // WinSock 謙猿
			return;
		}
		else if (nowStatus == "turn") {
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gotoxy(38, 2);
			cout << nickname << " - 綵堅啪歜" << endl;
			gotoxy(20, 1);
			cout << "忙式式式式式式式式式式式式忖";
			gotoxy(20, 2);
			cout << "弛  釭陛晦    弛";
			gotoxy(20, 3);
			cout << "戌式式式式式式式式式式式式戎";
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
						// '#' 晦遽戲煎 碟й
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}
						// だ諒 唸婁 轎溘
						for (size_t i = 0; i < parts.size(); ++i) {
							//cout << "睡碟 " << i + 1 << ": " << parts[i] << "\n";
						}
						nowStatus = parts[0];
						receivedMessage.clear();  // 詭衛雖 籀葬 �� 綠遺
						//system("cls");
						gotoxy(38, 2);
						cout << nickname << " - 綵堅啪歜" << endl;
						gotoxy(20, 1);
						cout << "忙式式式式式式式式式式式式忖";
						gotoxy(20, 2);
						cout << "弛  釭陛晦    弛";
						gotoxy(20, 3);
						cout << "戌式式式式式式式式式式式式戎";
						gotoxy(38, 3);
						user.multiPrintBoard(0);
						//opponent.multiPrintBoard(1);
					}
				}
				if (result==0 && nowStatus == "turn" && stoi(parts[2]) == myClientID) {
					gotoxy(38, 3);
					cout << "渡褐 離滔殮棲棻." << endl;
					int mynum = user.multiChooseNum(count);
					if (mynum == -1) {
						gotoxy(38, 3);
						cout << "詩じ 綵堅 啪歜擊 醞欽м棲棻." << endl;
						gotoxy(36, 4);
						cout << "1蟾 �� 詭檣詭景煎 檜翕м棲棻." << endl;
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
					cout << "鼻渠 離滔殮棲棻." << endl;
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
						// '#' 晦遽戲煎 碟й
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}

						if (parts[0] == "new" || parts[0] == "start") {
							gotoxy(32, 3);
							cout << "熱塊 諫猿! 3蟾 �� 棻擠 啪歜擊 霞чм棲棻";
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
					cout << "鼻渠寞 熱塊擊 晦棻葬朝 醞" << endl;
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
	WSACleanup();  // WinSock 謙猿
	return;
} 

//啪歜 唸婁 �挫恉炴� л熱
int BingoGame::multiResultGame(BingoBoard& user, BingoBoard& opponent)
{
	if (user.checkWin() == -1 && opponent.checkWin() == -1) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "鼠蝓睡, 霞ч碳陛 ";		
		return -2;
	}
	else if (user.checkWin() > opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "餌辨濠曖 蝓葬    ";
		return 1;
	}
	else if (user.checkWin() < opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "鼻渠寞曖 蝓葬    ";
		return -1;
	}
	else if (user.checkWin() > 0 && opponent.checkWin() > 0 && (user.checkWin() == opponent.checkWin())) {
		gotoxy(42, 4);
		cout << "鼠蝓睡, 啗樓 霞ч";
		return 0;
	}
	else
		return 0;
}

//啪歜檜 部釭堅 だ橾 蟾晦�� ж堅 葬Ы溯檜ж剪釭 詭檣�飛� 陛朝 л熱
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
		cout << "忙式式式式式式式式式式式式忖";
		gotoxy(42, 7);
		cout << "弛 詭檣詭景煎 弛";
		gotoxy(42, 8);
		cout << "戌式式式式式式式式式式式式戎";

		gotoxy(42, 10);
		cout << "忙式式式式式式式式式式式式忖";
		gotoxy(42, 11);
		cout << "弛  棻衛ж晦  弛";
		gotoxy(42, 12);
		cout << "戌式式式式式式式式式式式式戎";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "忙式式式式式式式式式式式式忖";
			gotoxy(42, 7);
			cout << "弛 詭檣詭景煎 弛";
			gotoxy(42, 8);
			cout << "戌式式式式式式式式式式式式戎";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "忙式式式式式式式式式式式式忖";
			gotoxy(42, 11);
			cout << "弛  棻衛ж晦  弛";
			gotoxy(42, 12);
			cout << "戌式式式式式式式式式式式式戎";
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