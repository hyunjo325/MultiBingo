#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <sstream>
#include <string>
#include <winsock2.h>  // Windows ���� ���̺귯��
#pragma comment(lib, "ws2_32.lib")  // ���� ���̺귯�� ��ũ

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
struct Message {
    int senderID = 0;      // �߽��� ID
    int receiverID = 0;    // ������ ID
    string content; // �޽��� ����
};
struct GameRoom {
    int player1ID;
    SOCKET player1Socket;
    int player2ID;
    SOCKET player2Socket;
    int player1Status = 0;
    int player2Status = 0;
    int gamecount = 0;
    int notendcnt[2] = { 0, 0 };
};
map<int, GameRoom> gameRooms; // ���� �� ���
mutex gameRoomsMutex;         // �� ����� ��ȣ�ϴ� ���ؽ�

map<int, SOCKET> clients; // Ŭ���̾�Ʈ ID�� ������ ����
mutex clientsMutex;


void handleClientDisconnection(int clientID) {
    lock_guard<mutex> lock(gameRoomsMutex);

    for (auto it = gameRooms.begin(); it != gameRooms.end(); ++it) {
        GameRoom& room = it->second;

        if ((room.player1Status != -1 || room.player2Status != -1) && room.player1ID == clientID || room.player2ID == clientID) {
            std::cout << "�÷��̾� " << clientID << "�� �������ϴ�. ���� �����մϴ�.\n";

            // ������ �÷��̾�� ���� ���� �˸�
            if (room.player1ID == clientID&& room.player2Status != -1) {
                send(room.player2Socket, "notice#��밡 ������ �������ϴ�.", strlen("notice#��밡 ������ �������ϴ�."), 0);
                closesocket(room.player2Socket);
            }
            else if(room.player2ID == clientID && room.player1Status != -1){
                send(room.player1Socket, "notice#��밡 ������ �������ϴ�.", strlen("notice#��밡 ������ �������ϴ�."), 0);
                closesocket(room.player1Socket);
            }

            // �� ����
            gameRooms.erase(it);
            break;
        }
    }
}

void handleClient(SOCKET clientSocket, int clientId) {
    static int boardsize = 0;
    int nowroomID = 0;
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            cout << clientId <<"Ŭ���̾�Ʈ ���� ����\n";
            if (gameRooms[nowroomID].player1ID == clientId) {
                gameRooms[nowroomID].player1Status = -1;
            }
            else if (gameRooms[nowroomID].player2ID == clientId) {
                gameRooms[nowroomID].player2Status = -1;
            }
            handleClientDisconnection(clientId);
            closesocket(clientSocket);
            break;
        }

        string receivedMessage;
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // ������ ������ ���� NULL ���� �߰�
            receivedMessage = string(buffer);
            cout << receivedMessage;
        }
        vector<std::string> parts;
        parts.clear();
        istringstream ss(receivedMessage);
        string item;
        // '#' �������� ����
        while (getline(ss, item, '#')) {
            parts.push_back(item);
        }
        // �Ľ� ��� ���
        if (parts[0] == "board") {
            nowroomID = stoi(parts[1]);
            lock_guard<mutex> lock(gameRoomsMutex);
            if (gameRooms[nowroomID].player1ID == stoi(parts[2])) {
                send(gameRooms[nowroomID].player2Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
                cout << "plaer1 ���� " << receivedMessage << "\n";
            }
            else if (gameRooms[nowroomID].player2ID == stoi(parts[2])) {
                send(gameRooms[nowroomID].player1Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
                cout << "plaer2 ���� " << receivedMessage << "\n";
            }
            
        }
        else if (parts[0] == "done") {
            nowroomID = stoi(parts[1]);
            lock_guard<mutex> lock(gameRoomsMutex);
            if (gameRooms[nowroomID].player1ID == stoi(parts[2])) {
                gameRooms[nowroomID].player1Status = 1;
            }
            else if (gameRooms[nowroomID].player2ID == stoi(parts[2])) {
                gameRooms[nowroomID].player2Status = 1;
            }
            if (gameRooms[nowroomID].player1Status == 1 && gameRooms[nowroomID].player2Status == 1) {
                cout << "���弼�ÿϷ�\n";
                string stmsg = "set";
                send(gameRooms[nowroomID].player1Socket, stmsg.c_str(), stmsg.length(), 0);
                send(gameRooms[nowroomID].player2Socket, stmsg.c_str(), stmsg.length(), 0);
                cout << nowroomID << " " << stmsg << "\n";
                //Sleep(1500);
                stmsg = "turn#";
                stmsg += parts[1];
                stmsg += "#";
                stmsg += to_string(gameRooms[nowroomID].player1ID);
                send(gameRooms[nowroomID].player1Socket, stmsg.c_str(), stmsg.length(), 0);
                send(gameRooms[nowroomID].player2Socket, stmsg.c_str(), stmsg.length(), 0);
                cout << nowroomID << " " << stmsg << "\n";
                gameRooms[nowroomID].gamecount = 1;
            }
        }
        else if (parts[0] == "num") {
            Sleep(100);
            nowroomID = stoi(parts[1]);
            lock_guard<mutex> lock(gameRoomsMutex);
            receivedMessage += "#";
            receivedMessage += to_string(gameRooms[nowroomID].gamecount);
            cout << receivedMessage<<"\n";
            send(gameRooms[nowroomID].player1Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
            send(gameRooms[nowroomID].player2Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
            cout << nowroomID << " " << receivedMessage << "\n";
        }
        else if (parts[0] == "notend") {
            cout << "notend\n";
            nowroomID = stoi(parts[1]);
            int clientID = stoi(parts[2]);
            lock_guard<mutex> lock(gameRoomsMutex);
            string stmsg = "turn#";
            stmsg += parts[1];
            stmsg += "#";
            Sleep(100);
            if (gameRooms[nowroomID].gamecount % 2 == 0) {
                if (gameRooms[nowroomID].player1ID== clientID) {
                    gameRooms[nowroomID].notendcnt[0] = 1;
                    cout << "0��\n";
                }
                else if (gameRooms[nowroomID].player2ID == clientID) {
                    gameRooms[nowroomID].notendcnt[1] = 1;
                    cout << "1��\n";
                }
                stmsg += to_string(gameRooms[nowroomID].player1ID);
            }
            else {
                if (gameRooms[nowroomID].player1ID == clientID) {
                    gameRooms[nowroomID].notendcnt[0] = 1;
                    cout << "0��\n";
                }
                else if (gameRooms[nowroomID].player2ID == clientID) {
                    gameRooms[nowroomID].notendcnt[1] = 1;
                    cout << "1��\n";
                }
                stmsg += to_string(gameRooms[nowroomID].player2ID);
            }
            send(clientSocket, stmsg.c_str(), stmsg.length(), 0);
            cout << nowroomID << " " << stmsg << "\n";
            if (gameRooms[nowroomID].notendcnt[0] == 1 && gameRooms[nowroomID].notendcnt[1] == 1) {
                gameRooms[nowroomID].notendcnt[0] = 0;
                gameRooms[nowroomID].notendcnt[1] = 0;
                gameRooms[nowroomID].gamecount++;
                cout << "gmc: " << gameRooms[nowroomID].gamecount << "\n";
            }
        }
        else if (parts[0] == "end") {
            nowroomID = stoi(parts[1]);
            int winnerID = stoi(parts[2]);
            int result = stoi(parts[3]);
            if (result == 1 && gameRooms[nowroomID].player1ID == winnerID) {
                gameRooms[nowroomID].player1Status = 9;
            }
            else if (result == 1 && gameRooms[nowroomID].player2ID == winnerID) {
                gameRooms[nowroomID].player2Status = 9;
            }
            if (gameRooms[nowroomID].player1Status == 9 || gameRooms[nowroomID].player2Status == 9) {
                cout << nowroomID << "�� �� ���� ����\n";
                gameRooms[nowroomID].player1Status = 9;
                gameRooms[nowroomID].player2Status = 9;
            }

        }
        else if (parts[0] == "restart") {
            nowroomID = stoi(parts[1]);
            int clientID = stoi(parts[2]);
            int restart = stoi(parts[3]);
            if (restart == 1 && gameRooms[nowroomID].player1ID == clientID) {
                gameRooms[nowroomID].player1Status = 1;
                if (gameRooms[nowroomID].player2Status != 1) {
                    gameRooms[nowroomID].gamecount = 1;
                }
            }
            else if (restart == 0 && gameRooms[nowroomID].player1ID == clientID) {
                cout << clientID << "�� Ŭ���̾�Ʈ ����\n";
                gameRooms[nowroomID].player1Status = -1;
                handleClientDisconnection(clientID);
                closesocket(clientSocket);
                break;
            }
            else if (restart == 1 && gameRooms[nowroomID].player2ID == clientID) {
                gameRooms[nowroomID].player2Status = 1;
                if (gameRooms[nowroomID].player1Status != 1) {
                    gameRooms[nowroomID].gamecount = 2;
                }
            }
            else if (restart == 0 && gameRooms[nowroomID].player2ID == clientID) {
                cout << clientID << "�� Ŭ���̾�Ʈ ����\n";
                gameRooms[nowroomID].player2Status = -1;
                handleClientDisconnection(clientID);
                closesocket(clientSocket);
                break;
            }
            if (gameRooms[nowroomID].player1Status == 1 && gameRooms[nowroomID].player2Status == 1) {
                boardsize = 0;
                //�� Ŭ���̾�Ʈ ��� ����� ����
                cout << nowroomID << "�� �� �����\n";
                if (gameRooms[nowroomID].gamecount == 1) {
                    string newmsg = "new#";
                    newmsg += to_string(1);
                    send(gameRooms[nowroomID].player1Socket, newmsg.c_str(), newmsg.length(), 0);
                    cout << clientID << " " << newmsg << "\n";
                    cout << "Ŭ���̾�Ʈ ��� ��: " << clientID << "\n";
                    newmsg += "new#";
                    newmsg += to_string(-1);
                    send(gameRooms[nowroomID].player2Socket, newmsg.c_str(), newmsg.length(), 0);
                }
                else if (gameRooms[nowroomID].gamecount == 2) {
                    string newmsg = "new#";
                    newmsg += to_string(1);
                    send(gameRooms[nowroomID].player2Socket, newmsg.c_str(), newmsg.length(), 0);
                    cout << clientID << " " << newmsg << "\n";
                    cout << "Ŭ���̾�Ʈ ��� ��: " << clientID << "\n";
                    newmsg = "new#";
                    newmsg += to_string(-1);
                    send(gameRooms[nowroomID].player1Socket, newmsg.c_str(), newmsg.length(), 0);
                }
            }

        }
        else if (parts[0] == "size") {
            boardsize = stoi(parts[2]);
            cout << "���� ũ�� " << boardsize << "\n";
            string stmsg = "start#";
            stmsg += to_string(nowroomID);
            stmsg += "#";
            stmsg += to_string(boardsize);
            // �濡 �ִ� �� ���� Ŭ���̾�Ʈ���� ����� �˸�
            send(gameRooms[nowroomID].player1Socket, stmsg.c_str(), stmsg.length(), 0);
            send(gameRooms[nowroomID].player2Socket, stmsg.c_str(), stmsg.length(), 0);
            gameRooms[nowroomID].gamecount = 1;
        }
        else {
            for (size_t i = 0; i < parts.size(); ++i) {
                cout << "�κ� " << i + 1 << ": " << parts[i] << "\n";
            }
        }
        //cout << "Ŭ���̾�ƮID" << msg.senderID<<"�κ��� �޽��� : " << msg.content << endl;
        receivedMessage.clear();
        string response = "�������� ó�� �Ϸ�\n";
        //send(clientSocket, response.c_str(), response.size(), 0);
    }
    lock_guard<mutex> lock(clientsMutex);
    clients.erase(clientId);
    closesocket(clientSocket);
}

int roomID = 0; // �� ID ī����
queue<pair<int, SOCKET>> waitingQueue; // ��� ���� Ŭ���̾�Ʈ ���

void assignClientToRoom(SOCKET clientSocket, int clientID) {
    lock_guard<mutex> lock(gameRoomsMutex);
    static int boardsize = 0;
    if (!waitingQueue.empty()) {
        // ��� ���� Ŭ���̾�Ʈ�� ������ ���ο� �� ����
        auto waitingClient = waitingQueue.front();
        waitingQueue.pop();

        GameRoom room;
        room.player1ID = waitingClient.first;
        room.player1Socket = waitingClient.second;
        room.player2ID = clientID;
        room.player2Socket = clientSocket;

        gameRooms[roomID] = room; // ���ο� �濡 �߰�
        while (boardsize == 0) { Sleep(1000); }
        cout << "�� ����: " << room.player1ID << "�� " << room.player2ID << "\n";
        string stmsg = "start#";
        stmsg += to_string(roomID++);
        stmsg += "#";
        stmsg += to_string(boardsize);
        cout << clientID << " " << stmsg << "\n";
        // �濡 �ִ� �� ���� Ŭ���̾�Ʈ���� ���� ���� �˸�
        send(room.player1Socket, stmsg.c_str(), stmsg.length(), 0);
        send(room.player2Socket, stmsg.c_str(), stmsg.length(), 0);
        Sleep(100);
    }
    else {
        // ��� ���� Ŭ���̾�Ʈ�� ������ ť�� �߰�
        waitingQueue.push({ clientID, clientSocket });
        string newmsg = "new#1";
        send(clientSocket, newmsg.c_str(), newmsg.length(), 0);
        cout << clientID << " " << newmsg << "\n";
        cout << "Ŭ���̾�Ʈ ��� ��: " << clientID << "\n";


        char buffer[BUFFER_SIZE];
        string receivedMessage;
        int boardsizeReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (boardsizeReceived > 0) {
            buffer[boardsizeReceived] = '\0';  // ������ ������ ���� NULL ���� �߰�
            receivedMessage = string(buffer);
            cout << receivedMessage;
        }
        vector<std::string> parts;
        parts.clear();
        istringstream ss(receivedMessage);
        string item;
        // '#' �������� ����
        while (getline(ss, item, '#')) {
            parts.push_back(item);
        }
        boardsize = stoi(parts[2]);
        cout << "���� ũ�� " << boardsize << "\n";
    }
}


int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // WinSock �ʱ�ȭ
    int clientIdx = 1;
    SOCKET serverSocket, clientSocket;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "���� ���� ����\n";
        return 1;
    }

    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "���ε� ����\n";
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        cerr << "������ ����\n";
        return 1;
    }

    cout << "������ Ŭ���̾�Ʈ�� ��ٸ��� ��...\n";

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Ŭ���̾�Ʈ ���� ����\n";
            continue;
        }


        cout << "Ŭ���̾�Ʈ �����\n";
        {
            lock_guard<mutex> lock(clientsMutex);
            clients.insert({ clientIdx, clientSocket });
        }
        string msg = to_string(clientIdx);
        send(clientSocket, msg.c_str(), msg.length(), 0);
        assignClientToRoom(clientSocket, clientIdx);
        thread clientThread(handleClient, clientSocket, clientIdx);
        clientThread.detach();  // �����带 �и��Ͽ� ���������� ����
        clientIdx++;
    }

    closesocket(serverSocket);
    WSACleanup();  // WinSock ����
    return 0;
}
