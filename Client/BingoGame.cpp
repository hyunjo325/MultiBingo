#pragma once
#include "BingoGame.h"

//���� �÷��� �Լ�
void BingoGame::play()
{
	system("mode con:cols=100 lines=24");
	gotoxy(38,1);
	cout << "������ - �������" << endl;
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

//�޴� �Լ�
int BingoGame::menu()
{
	system("cls");
	setCursorView(false);
	gotoxy(38, 1);
	cout << "������ - �������" << endl;
	char input = NULL;
	int menuNum = 0;
	
	long length = 0;
	while (input != 'e') {
		gotoxy(10, 3);
		cout << "������������������������������������";
		gotoxy(10, 4);
		cout << "��  1) �� ����    ��";
		gotoxy(10, 5);
		cout << "������������������������������������";
		gotoxy(30, 3);
		cout << "������������������������������������";
		gotoxy(30, 4);
		cout << "��  2) �̾��ϱ�   ��";
		gotoxy(30, 5);
		cout << "������������������������������������";
		gotoxy(50, 3);
		cout << "������������������������������������";
		gotoxy(50, 4);
		cout << "��  3) ��Ƽ�÷��� ��";
		gotoxy(50, 5);
		cout << "������������������������������������";
		gotoxy(70, 3);
		cout << "������������������������������������";
		gotoxy(70, 4);
		cout << "��  4) ����       ��";
		gotoxy(70, 5);
		cout << "������������������������������������";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(10, 3);
			cout << "������������������������������������";
			gotoxy(10, 4);
			cout << "��  1) �� ����    ��";
			gotoxy(10, 5);
			cout << "������������������������������������";
			break;
		case 1:
			if (length == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			}
			gotoxy(30, 3);
			cout << "������������������������������������";
			gotoxy(30, 4);
			cout << "��  2) �̾��ϱ�   ��";
			gotoxy(30, 5);
			cout << "������������������������������������";
			break;
		case 2:
			gotoxy(50, 3);
			cout << "������������������������������������";
			gotoxy(50, 4);
			cout << "��  3) ��Ƽ�÷��� ��";
			gotoxy(50, 5);
			cout << "������������������������������������";
			break;
		case 3:
			gotoxy(70, 3);
			cout << "������������������������������������";
			gotoxy(70, 4);
			cout << "��  4) ����       ��";
			gotoxy(70, 5);
			cout << "������������������������������������";
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
				cout << "���� ���� ����" << endl;
				return 1; // ���� �ڵ� ��ȯ
			}

			fout.close();
		}
		fin.seekg(0, ios::end);
		length = (long)fin.tellg();
		if (length == 0 && menuNum == 1) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			gotoxy(30, 3);
			cout << "������������������������������������";
			gotoxy(30, 4);
			cout << "��  2) �̾��ϱ�   ��";
			gotoxy(30, 5);
			cout << "������������������������������������";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			input = NULL;
			continue;
		}
	}
	
	return menuNum;
}

//��������� �����ϴ� �Լ�
void BingoGame::playGame(BingoBoard& user, BingoBoard& com)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("cls");
	gotoxy(38, 2);
	cout << "������ - �������" << endl;
	gotoxy(20, 1);
	cout << "����������������������������";
	gotoxy(20, 2);
	cout << "��  �����ϱ�  ��";
	gotoxy(20, 3);
	cout << "����������������������������";
	user.printBoard(0);
	//com.printBoard(1);
	while (true) {
		int mynum = user.chooseNum(count);
		if (mynum == -1) {
			user.saveBoard(0, count);
			com.saveBoard(1, count);
			gotoxy(38, 3);
			cout << "��������� ����Ǿ����ϴ�." << endl;
			gotoxy(36, 4);
			cout << "�ڵ����� ���θ޴��� �̵��մϴ�." << endl;
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

//�� ������ �����ϱ� ���� �ʱ�ȭ�ϴ� �Լ�
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

//������ ũ�⸦ ���ϴ� �Լ�
int BingoGame::chooseSize()
{
	char input = NULL;
	int size = 3;
	system("cls");
	gotoxy(38, 1);
	cout << "������ - �������" << endl;
	gotoxy(38, 3);
	cout << "�������� ũ�⸦ ���� �ּ���";
	while (input != 'e') {
		for (int i = 0; i < 7; i++) {
			gotoxy(22 + i * 8, 5);
			cout << "����������������";
			gotoxy(22 + i * 8, 6);
			cout << "�� " << i + 3 << "��" << i + 3 << "  ��";
			gotoxy(22 + i * 8, 7);
			cout << "����������������";
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		gotoxy(22 + (size - 3) * 8, 5);
		cout << "����������������";
		gotoxy(22 + (size - 3) * 8, 6);
		cout << "�� " << size << "��" << size << "  ��";
		gotoxy(22 + (size - 3) * 8, 7);
		cout << "����������������";

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

//�̾��ϱ� ���� ũ��� ������ �ҷ����� �Լ�
void BingoGame::resumeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "������ - �������" << endl;
	gotoxy(38, 2);
	cout << "��������� �ٽ� ���۵Ǿ����ϴ�." << endl;
	const char* file = "./userdata.dat";
	ifstream fin(file, ios::in | ios::binary);
	if (!fin) {
		cout << "�������� ���� ����";
		return;
	}
	fin.read((char*)(&count), sizeof(count));
	fin.read((char*)(&size), sizeof(size));
	fin.close();
	
	return;
}

//���α׷��� �����ϴ� �Լ�
void BingoGame::closeGame()
{
	system("cls");
	gotoxy(38, 1);
	cout << "������ - �������" << endl;
	gotoxy(38, 2);
	cout << "��������� ����Ǿ����ϴ�." << endl;
	
}

//Ű���� �Է��� �޴� �Լ�
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

//���� ��� Ȯ���ϴ� �Լ�
int BingoGame::resultGame(BingoBoard& u, BingoBoard& c)
{
	if (u.checkWin() == -1 && c.checkWin() == -1) {
		gotoxy(42, 3);
		cout << "���º�, ����Ұ� ";
		return -2;
	}
	else if (u.checkWin() > c.checkWin()) {
		gotoxy(42, 3);
		cout << "������� �¸�    ";
		return 1;
	}
	else if (u.checkWin() < c.checkWin()) {
		gotoxy(42, 3);
		cout << "��ǻ���� �¸�    ";
		return -1;
	}
	else if (u.checkWin() > 0 && c.checkWin() > 0 && (u.checkWin() == c.checkWin())) {
		gotoxy(42, 3);
		cout << "���º�, ��� ����";
		return 0;
	}
	else
		return 0;
}

//������ ������ ���� �ʱ�ȭ �ϰ� ���÷����ϰų� ����ȭ�� ���� �Լ�
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
		cout << "����������������������������";
		gotoxy(42, 7);
		cout << "�� ���θ޴��� ��";
		gotoxy(42, 8);
		cout << "����������������������������";

		gotoxy(42, 10);
		cout << "����������������������������";
		gotoxy(42, 11);
		cout << "��  ���÷���  ��";
		gotoxy(42, 12);
		cout << "����������������������������";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "����������������������������";
			gotoxy(42, 7);
			cout << "�� ���θ޴��� ��";
			gotoxy(42, 8);
			cout << "����������������������������";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "����������������������������";
			gotoxy(42, 11);
			cout << "��  ���÷���  ��";
			gotoxy(42, 12);
			cout << "����������������������������";
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

// �����κ��� �޽����� �����ϴ� �Լ�
void BingoGame::receiveMessages(SOCKET clientSocket) {
	char buffer[BUFFER_SIZE];
	receivedMessage.clear();
	while (!stopFlag) {
		int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

		if (bytesReceived <= 0) {
			gotoxy(38, 3);
			cerr << "�������� ������ ����Ǿ����ϴ�.\n";
			Sleep(1000);
			stopFlag = true;
			break;
		}

		// ���� �޽����� ���� ���� ó��
		{
			lock_guard<mutex> lock(messageMutex);
			if (bytesReceived > 0) {
				buffer[bytesReceived] = '\0';  // ������ ������ ���� NULL ���� �߰�
				receivedMessage = string(buffer);
				if (receivedMessage == "notice#��밡 ������ �������ϴ�.") {
					gotoxy(38, 2);
					cout << "��밡 ������ �������ϴ�." << endl;
					Sleep(1000);
					stopFlag = true; 
					closesocket(clientSocket);  // ���� �ݱ�
					WSACleanup();
					break;
				}
				vector<string> parts;
				istringstream ss(receivedMessage);
				string item;
				// '#' �������� ����
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}
				// �Ľ� ��� ���
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "thread�κ� " << i + 1 << ": " << parts[i] << "\n";
				}
				gotoxy(0, 0);
				//cout << receivedMessage;
			}
		}
	}
	closesocket(clientSocket);  // ���� �ݱ�
	WSACleanup();  // Winsock ����
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
	WSAStartup(MAKEWORD(2, 2), &wsaData);  // WinSock �ʱ�ȭ

	char buffer[BUFFER_SIZE];
	SOCKET clientSocket;
	struct sockaddr_in serverAddr;
	int myClientID = -1;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "���� ���� ����\n";
		return;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	// inet_addr�� ����Ͽ� IP �ּҸ� ��ȯ
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // ���⼭ IP �ּҸ� �����մϴ�.

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "���� ����\n";
		return;
	}

	char input = NULL;
	int roomid = 0;
	stopFlag = false;
	cout << "������ ����Ǿ����ϴ�.\n";
	int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // ������ ������ ���� NULL ���� �߰�
		myClientID = stoi(string(buffer));
	}
	cout << "Ŭ���̾�Ʈ ID : " << myClientID << "\n";
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

				// '#' �������� ����
				while (getline(ss, item, '#')) {
					parts.push_back(item);
				}

				// �Ľ� ��� ���
				for (size_t i = 0; i < parts.size(); ++i) {
					//cout << "while�κ� " << i + 1 << ": " << parts[i] << "\n";
				}
				nowStatus = parts[0];
				receivedMessage.clear();  // �޽��� ó�� �� ���
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
				cout << "�ٸ� �����ڸ� ��ٸ�����..\n";
				cout << "�������� ũ�⸦ ���� �ּ���";
				while (input != 'e') {
					for (int i = 0; i < 7; i++) {
						gotoxy(22 + i * 8, 5);
						cout << "����������������";
						gotoxy(22 + i * 8, 6);
						cout << "�� " << i + 3 << "��" << i + 3 << "  ��";
						gotoxy(22 + i * 8, 7);
						cout << "����������������";
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

					gotoxy(22 + (size - 3) * 8, 5);
					cout << "����������������";
					gotoxy(22 + (size - 3) * 8, 6);
					cout << "�� " << size << "��" << size << "  ��";
					gotoxy(22 + (size - 3) * 8, 7);
					cout << "����������������";

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
				cout << "������ ũ�� : " << size << "���� �Ϸ�\n";
				cout << "�ٸ� �����ڸ� ��ٸ�����..\n";
				nowStatus = "NULL";
			}
			else {
				gotoxy(0, 0);
				cout << "���� �ٽ��ϱ⸦ ���� ������ ������ ũ�⸦ ���ϰ� �ֽ��ϴ�.";
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
			cout << "�������� ũ�⸦ " << size << "��" << size << "�� ������ �����մϴ�.\n";
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
			//cout << "��� ���� ���� �Ϸ�\n";
			string setmsg = "done#";
			setmsg += to_string(roomid);
			setmsg += "#";
			setmsg += to_string(myClientID);
			send(clientSocket, setmsg.c_str(), setmsg.length(), 0);
			nowStatus = "NULL";
		}
		else if (nowStatus == "set") {
			gotoxy(38, 2);
			cout << "��� �� ��Ƽ ���� ������ ���۵˴ϴ�" << endl;
			nowStatus = "NULL";
			Sleep(1000);
			system("cls");
		}
		else if (nowStatus == "notice") {
			nowStatus = "NULL";
			gotoxy(36, 4);
			cout << "�ڵ����� ���θ޴��� �̵��մϴ�?" << endl;
			Sleep(1000);
			size = 0;
			result = 0;
			restart = -1;
			stopFlag = false;
			WSACleanup();  // WinSock ����
			return;
		}
		else if (nowStatus == "turn") {
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gotoxy(38, 2);
			cout << "������ - �������" << endl;
			gotoxy(20, 1);
			cout << "����������������������������";
			gotoxy(20, 2);
			cout << "��  ������    ��";
			gotoxy(20, 3);
			cout << "����������������������������";
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
						// '#' �������� ����
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}
						// �Ľ� ��� ���
						for (size_t i = 0; i < parts.size(); ++i) {
							//cout << "�κ� " << i + 1 << ": " << parts[i] << "\n";
						}
						nowStatus = parts[0];
						receivedMessage.clear();  // �޽��� ó�� �� ���
						//system("cls");
						gotoxy(38, 2);
						cout << "������ - �������" << endl;
						gotoxy(20, 1);
						cout << "����������������������������";
						gotoxy(20, 2);
						cout << "��  ������    ��";
						gotoxy(20, 3);
						cout << "����������������������������";
						gotoxy(38, 3);
						user.multiPrintBoard(0);
						//opponent.multiPrintBoard(1);
					}
				}
				if (result==0 && nowStatus == "turn" && stoi(parts[2]) == myClientID) {
					gotoxy(38, 3);
					cout << "��� �����Դϴ�." << endl;
					int mynum = user.multiChooseNum(count);
					if (mynum == -1) {
						gotoxy(38, 3);
						cout << "��Ƽ ���� ������ �ߴ��մϴ�." << endl;
						gotoxy(36, 4);
						cout << "1�� �� ���θ޴��� �̵��մϴ�." << endl;
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
					cout << "��� �����Դϴ�." << endl;
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
						// '#' �������� ����
						while (getline(ss, item, '#')) {
							parts.push_back(item);
						}

						if (parts[0] == "new" || parts[0] == "start") {
							gotoxy(32, 3);
							cout << "���� �Ϸ�! 3�� �� ���� ������ �����մϴ�";
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
					cout << "���� ������ ��ٸ��� ��" << endl;
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
	WSACleanup();  // WinSock ����
	return;
} 

//���� ��� Ȯ���ϴ� �Լ�
int BingoGame::multiResultGame(BingoBoard& user, BingoBoard& opponent)
{
	if (user.checkWin() == -1 && opponent.checkWin() == -1) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "���º�, ����Ұ� ";		
		return -2;
	}
	else if (user.checkWin() > opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "������� �¸�    ";
		return 1;
	}
	else if (user.checkWin() < opponent.checkWin()) {
		gotoxy(38, 3);
		cout << "                ";
		gotoxy(42, 4);
		cout << "������ �¸�    ";
		return -1;
	}
	else if (user.checkWin() > 0 && opponent.checkWin() > 0 && (user.checkWin() == opponent.checkWin())) {
		gotoxy(42, 4);
		cout << "���º�, ��� ����";
		return 0;
	}
	else
		return 0;
}

//������ ������ ���� �ʱ�ȭ �ϰ� ���÷����ϰų� ����ȭ�� ���� �Լ�
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
		cout << "����������������������������";
		gotoxy(42, 7);
		cout << "�� ���θ޴��� ��";
		gotoxy(42, 8);
		cout << "����������������������������";

		gotoxy(42, 10);
		cout << "����������������������������";
		gotoxy(42, 11);
		cout << "��  �ٽ��ϱ�  ��";
		gotoxy(42, 12);
		cout << "����������������������������";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		switch (menuNum) {
		case 0:
			gotoxy(42, 6);
			cout << "����������������������������";
			gotoxy(42, 7);
			cout << "�� ���θ޴��� ��";
			gotoxy(42, 8);
			cout << "����������������������������";
			break;
		case 1:
			gotoxy(42, 10);
			cout << "����������������������������";
			gotoxy(42, 11);
			cout << "��  �ٽ��ϱ�  ��";
			gotoxy(42, 12);
			cout << "����������������������������";
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