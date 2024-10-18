#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <sstream>
#include <string>
#include <winsock2.h>  // Windows 소켓 라이브러리
#pragma comment(lib, "ws2_32.lib")  // 소켓 라이브러리 링크

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
struct Message {
    int senderID = 0;      // 발신자 ID
    int receiverID = 0;    // 수신자 ID
    string content; // 메시지 내용
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
map<int, GameRoom> gameRooms; // 게임 방 목록
mutex gameRoomsMutex;         // 방 목록을 보호하는 뮤텍스

map<int, SOCKET> clients; // 클라이언트 ID와 소켓을 매핑
mutex clientsMutex;


void handleClientDisconnection(int clientID) {
    lock_guard<mutex> lock(gameRoomsMutex);

    for (auto it = gameRooms.begin(); it != gameRooms.end(); ++it) {
        GameRoom& room = it->second;

        if ((room.player1Status != -1 || room.player2Status != -1) && room.player1ID == clientID || room.player2ID == clientID) {
            std::cout << "플레이어 " << clientID << "가 나갔습니다. 방을 제거합니다.\n";

            // 나머지 플레이어에게 연결 종료 알림
            if (room.player1ID == clientID&& room.player2Status != -1) {
                send(room.player2Socket, "notice#상대가 게임을 나갔습니다.", strlen("notice#상대가 게임을 나갔습니다."), 0);
                closesocket(room.player2Socket);
            }
            else if(room.player2ID == clientID && room.player1Status != -1){
                send(room.player1Socket, "notice#상대가 게임을 나갔습니다.", strlen("notice#상대가 게임을 나갔습니다."), 0);
                closesocket(room.player1Socket);
            }

            // 방 삭제
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
            cout << clientId <<"클라이언트 연결 종료\n";
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
            buffer[bytesReceived] = '\0';  // 수신한 데이터 끝에 NULL 문자 추가
            receivedMessage = string(buffer);
            cout << receivedMessage;
        }
        vector<std::string> parts;
        parts.clear();
        istringstream ss(receivedMessage);
        string item;
        // '#' 기준으로 분할
        while (getline(ss, item, '#')) {
            parts.push_back(item);
        }
        // 파싱 결과 출력
        if (parts[0] == "board") {
            nowroomID = stoi(parts[1]);
            lock_guard<mutex> lock(gameRoomsMutex);
            if (gameRooms[nowroomID].player1ID == stoi(parts[2])) {
                send(gameRooms[nowroomID].player2Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
                cout << "plaer1 보드 " << receivedMessage << "\n";
            }
            else if (gameRooms[nowroomID].player2ID == stoi(parts[2])) {
                send(gameRooms[nowroomID].player1Socket, receivedMessage.c_str(), receivedMessage.length(), 0);
                cout << "plaer2 보드 " << receivedMessage << "\n";
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
                cout << "보드세팅완료\n";
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
                    cout << "0업\n";
                }
                else if (gameRooms[nowroomID].player2ID == clientID) {
                    gameRooms[nowroomID].notendcnt[1] = 1;
                    cout << "1업\n";
                }
                stmsg += to_string(gameRooms[nowroomID].player1ID);
            }
            else {
                if (gameRooms[nowroomID].player1ID == clientID) {
                    gameRooms[nowroomID].notendcnt[0] = 1;
                    cout << "0업\n";
                }
                else if (gameRooms[nowroomID].player2ID == clientID) {
                    gameRooms[nowroomID].notendcnt[1] = 1;
                    cout << "1업\n";
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
                cout << nowroomID << "번 방 게임 종료\n";
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
                cout << clientID << "번 클라이언트 종료\n";
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
                cout << clientID << "번 클라이언트 종료\n";
                gameRooms[nowroomID].player2Status = -1;
                handleClientDisconnection(clientID);
                closesocket(clientSocket);
                break;
            }
            if (gameRooms[nowroomID].player1Status == 1 && gameRooms[nowroomID].player2Status == 1) {
                boardsize = 0;
                //두 클라이언트 모두 재시작 동의
                cout << nowroomID << "번 방 재시작\n";
                if (gameRooms[nowroomID].gamecount == 1) {
                    string newmsg = "new#";
                    newmsg += to_string(1);
                    send(gameRooms[nowroomID].player1Socket, newmsg.c_str(), newmsg.length(), 0);
                    cout << clientID << " " << newmsg << "\n";
                    cout << "클라이언트 대기 중: " << clientID << "\n";
                    newmsg += "new#";
                    newmsg += to_string(-1);
                    send(gameRooms[nowroomID].player2Socket, newmsg.c_str(), newmsg.length(), 0);
                }
                else if (gameRooms[nowroomID].gamecount == 2) {
                    string newmsg = "new#";
                    newmsg += to_string(1);
                    send(gameRooms[nowroomID].player2Socket, newmsg.c_str(), newmsg.length(), 0);
                    cout << clientID << " " << newmsg << "\n";
                    cout << "클라이언트 대기 중: " << clientID << "\n";
                    newmsg = "new#";
                    newmsg += to_string(-1);
                    send(gameRooms[nowroomID].player1Socket, newmsg.c_str(), newmsg.length(), 0);
                }
            }

        }
        else if (parts[0] == "size") {
            boardsize = stoi(parts[2]);
            cout << "보드 크기 " << boardsize << "\n";
            string stmsg = "start#";
            stmsg += to_string(nowroomID);
            stmsg += "#";
            stmsg += to_string(boardsize);
            // 방에 있는 두 명의 클라이언트에게 재시작 알림
            send(gameRooms[nowroomID].player1Socket, stmsg.c_str(), stmsg.length(), 0);
            send(gameRooms[nowroomID].player2Socket, stmsg.c_str(), stmsg.length(), 0);
            gameRooms[nowroomID].gamecount = 1;
        }
        else {
            for (size_t i = 0; i < parts.size(); ++i) {
                cout << "부분 " << i + 1 << ": " << parts[i] << "\n";
            }
        }
        //cout << "클라이언트ID" << msg.senderID<<"로부터 메시지 : " << msg.content << endl;
        receivedMessage.clear();
        string response = "서버에서 처리 완료\n";
        //send(clientSocket, response.c_str(), response.size(), 0);
    }
    lock_guard<mutex> lock(clientsMutex);
    clients.erase(clientId);
    closesocket(clientSocket);
}

int roomID = 0; // 방 ID 카운터
queue<pair<int, SOCKET>> waitingQueue; // 대기 중인 클라이언트 목록

void assignClientToRoom(SOCKET clientSocket, int clientID) {
    lock_guard<mutex> lock(gameRoomsMutex);
    static int boardsize = 0;
    if (!waitingQueue.empty()) {
        // 대기 중인 클라이언트가 있으면 새로운 방 생성
        auto waitingClient = waitingQueue.front();
        waitingQueue.pop();

        GameRoom room;
        room.player1ID = waitingClient.first;
        room.player1Socket = waitingClient.second;
        room.player2ID = clientID;
        room.player2Socket = clientSocket;

        gameRooms[roomID] = room; // 새로운 방에 추가
        while (boardsize == 0) { Sleep(1000); }
        cout << "방 생성: " << room.player1ID << "와 " << room.player2ID << "\n";
        string stmsg = "start#";
        stmsg += to_string(roomID++);
        stmsg += "#";
        stmsg += to_string(boardsize);
        cout << clientID << " " << stmsg << "\n";
        // 방에 있는 두 명의 클라이언트에게 게임 시작 알림
        send(room.player1Socket, stmsg.c_str(), stmsg.length(), 0);
        send(room.player2Socket, stmsg.c_str(), stmsg.length(), 0);
        Sleep(100);
    }
    else {
        // 대기 중인 클라이언트가 없으면 큐에 추가
        waitingQueue.push({ clientID, clientSocket });
        string newmsg = "new#1";
        send(clientSocket, newmsg.c_str(), newmsg.length(), 0);
        cout << clientID << " " << newmsg << "\n";
        cout << "클라이언트 대기 중: " << clientID << "\n";


        char buffer[BUFFER_SIZE];
        string receivedMessage;
        int boardsizeReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (boardsizeReceived > 0) {
            buffer[boardsizeReceived] = '\0';  // 수신한 데이터 끝에 NULL 문자 추가
            receivedMessage = string(buffer);
            cout << receivedMessage;
        }
        vector<std::string> parts;
        parts.clear();
        istringstream ss(receivedMessage);
        string item;
        // '#' 기준으로 분할
        while (getline(ss, item, '#')) {
            parts.push_back(item);
        }
        boardsize = stoi(parts[2]);
        cout << "보드 크기 " << boardsize << "\n";
    }
}


int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // WinSock 초기화
    int clientIdx = 1;
    SOCKET serverSocket, clientSocket;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "소켓 생성 실패\n";
        return 1;
    }

    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "바인딩 실패\n";
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        cerr << "리스닝 실패\n";
        return 1;
    }

    cout << "서버가 클라이언트를 기다리는 중...\n";

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "클라이언트 연결 실패\n";
            continue;
        }


        cout << "클라이언트 연결됨\n";
        {
            lock_guard<mutex> lock(clientsMutex);
            clients.insert({ clientIdx, clientSocket });
        }
        string msg = to_string(clientIdx);
        send(clientSocket, msg.c_str(), msg.length(), 0);
        assignClientToRoom(clientSocket, clientIdx);
        thread clientThread(handleClient, clientSocket, clientIdx);
        clientThread.detach();  // 스레드를 분리하여 독립적으로 실행
        clientIdx++;
    }

    closesocket(serverSocket);
    WSACleanup();  // WinSock 종료
    return 0;
}
