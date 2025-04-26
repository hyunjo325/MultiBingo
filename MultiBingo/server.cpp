#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <sstream>
#include <string>
#include <asio.hpp>
#include <bson/bson.h>
#include <mongoc/mongoc.h>


#define PORT 8080
#define BUFFER_SIZE 1024
using asio::ip::tcp;
using namespace std;
struct Message {
    int senderID = 0;      // 발신자 ID
    int receiverID = 0;    // 수신자 ID
    string content; // 메시지 내용
};
struct UserInfo {
    string nickname;
    int wins;
    int losses;
    int clientID;
};
struct GameRoom {
    int player1ID;
    shared_ptr<tcp::socket> player1Socket;
    int player2ID;
    shared_ptr<tcp::socket> player2Socket;
    int player1Status = 0;
    int player2Status = 0;
    int gamecount = 0;
    int notendcnt[2] = { 0, 0 };
    UserInfo player1info;
    UserInfo player2info;
};
map<int, GameRoom> gameRooms; // 게임 방 목록
mutex gameRoomsMutex;         // 방 목록을 보호하는 뮤텍스

map<int, shared_ptr<tcp::socket>> clients; // 클라이언트 ID와 소켓을 매핑
mutex clientsMutex;

int roomID = 0; // 방 ID 카운터
queue<pair<UserInfo, shared_ptr<tcp::socket>>> waitingQueue; // 대기 중인 클라이언트 목록

void handleClientDisconnection(int clientID) {
    cout << "[DEBUG] disconnection 진입\n";
    //lock_guard<mutex> lock(gameRoomsMutex);

    for (auto it = gameRooms.begin(); it != gameRooms.end(); ++it) {
        GameRoom& room = it->second;

        if ((room.player1ID == clientID || room.player2ID == clientID) &&
            (room.player1Status != -1 || room.player2Status != -1)) {

            cout << "[DEBUG] 유효한 방 찾음, 클라이언트 ID: " << clientID << "\n";

            // 상대 소켓을 안전하게 추출
            shared_ptr<tcp::socket> otherSocket =
                (room.player1ID == clientID) ? room.player2Socket : room.player1Socket;

            // 알림 및 소켓 종료 처리 (예외 방지)
            try {
                if (otherSocket && otherSocket->is_open()) {
                    asio::write(*otherSocket, asio::buffer("notice#상대가 게임을 나갔습니다."));
                    otherSocket->shutdown(tcp::socket::shutdown_both);
                    otherSocket->close();
                }
            }
            catch (const std::exception& e) {
                cerr << "[ERROR] 상대 소켓 알림 실패: " << e.what() << "\n";
            }

            // 방 제거
            gameRooms.erase(it);
            cout << "[DEBUG] 방 제거 완료\n";
            return;
        }
    }

    cout << "[DEBUG] disconnection 대상 없음\n";
}



pair<bool, pair<int, int>> loginOrRegisterNickname(shared_ptr<tcp::socket> clientSocket, const string& nickname) {
    mongoc_client_t* mongoClient = mongoc_client_new("mongodb://localhost:27017");
    if (!mongoClient) {
        cerr << "[MongoDB] 클라이언트 생성 실패" << endl;
        return { false, {0, 0} };
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(mongoClient, "bingo_db", "players");
    bson_t* query = BCON_NEW("nickname", BCON_UTF8(nickname.c_str()));
    mongoc_cursor_t* cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t* result;
    bson_iter_t iter;
    int wins = 0, losses = 0;

    if (mongoc_cursor_next(cursor, &result)) {
        // 기존 사용자 로그인
        if (bson_iter_init_find(&iter, result, "win") && BSON_ITER_HOLDS_INT32(&iter))
            wins = bson_iter_int32(&iter);
        if (bson_iter_init_find(&iter, result, "lose") && BSON_ITER_HOLDS_INT32(&iter))
            losses = bson_iter_int32(&iter);

        string msg = "login#" + nickname + "#" + to_string(wins) + "#" + to_string(losses);
        cout << "[MongoDB] 로그인 성공: " << msg << endl;
        asio::write(*clientSocket, asio::buffer(msg));
    }
    else {
        // 신규 사용자 등록
        bson_t* doc = BCON_NEW(
            "nickname", BCON_UTF8(nickname.c_str()),
            "win", BCON_INT32(0),
            "lose", BCON_INT32(0)
        );
        mongoc_collection_insert_one(collection, doc, NULL, NULL, NULL);
        bson_destroy(doc);

        string msg = "register#" + nickname + "#0#0";
        cout << "[MongoDB] 회원가입 완료: " << msg << endl;
        asio::write(*clientSocket, asio::buffer(msg));
    }

    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(mongoClient);

    return { true, {wins, losses} };
}

void updateWinLoss(const string& nickname, bool isWin) {
    mongoc_client_t* client = mongoc_client_new("mongodb://localhost:27017");
    if (!client) {
        cerr << "[MongoDB] 클라이언트 연결 실패" << endl;
        return;
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(client, "bingo_db", "players");

    bson_t* query = BCON_NEW("nickname", BCON_UTF8(nickname.c_str()));
    bson_t* update = bson_new();
    bson_t inc;  // '$inc' 필드에 들어갈 문서

    BSON_APPEND_DOCUMENT_BEGIN(update, "$inc", &inc);
    if (isWin) {
        BSON_APPEND_INT32(&inc, "win", 1);
    }
    else {
        BSON_APPEND_INT32(&inc, "lose", 1);
    }
    bson_append_document_end(update, &inc);


    bson_error_t error;
    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        cerr << "[MongoDB] 승패 업데이트 실패: " << error.message << endl;
    }

    // 정리
    bson_destroy(query);
    bson_destroy(update);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
}

void handleClient(shared_ptr<tcp::socket> clientSocket, UserInfo clientInfo) {
    static int boardsize = 0;
    int nowroomID = 0;
    char buffer[BUFFER_SIZE];

    while (true) {
        asio::error_code ec;
        size_t len = clientSocket->read_some(asio::buffer(buffer), ec);
        if (ec || len == 0) {
            cout << clientInfo.clientID << " 클라이언트 연결 종료\n";
            handleClientDisconnection(clientInfo.clientID);
            clientSocket->close();
            break;
        }
        buffer[len] = '\0';
        cout << buffer << endl;
        string receivedMessage(buffer);
        istringstream ss(receivedMessage);
        string item;
        vector<string> parts;
        while (getline(ss, item, '#')) parts.push_back(item);

        if (parts.empty()) continue;

        lock_guard<mutex> lock(gameRoomsMutex);
        if (parts[0] == "board") {
            nowroomID = stoi(parts[1]);
            int senderID = stoi(parts[2]);
            shared_ptr<tcp::socket> targetSocket = (gameRooms[nowroomID].player1ID == senderID)
                ? gameRooms[nowroomID].player2Socket
                : gameRooms[nowroomID].player1Socket;
            asio::write(*targetSocket, asio::buffer(receivedMessage));
        }
        else if (parts[0] == "done") {
            nowroomID = stoi(parts[1]);
            int senderID = stoi(parts[2]);
            if (gameRooms[nowroomID].player1ID == senderID) gameRooms[nowroomID].player1Status = 1;
            if (gameRooms[nowroomID].player2ID == senderID) gameRooms[nowroomID].player2Status = 1;
            if (gameRooms[nowroomID].player1Status == 1 && gameRooms[nowroomID].player2Status == 1) {
                string msg = "set";
                asio::write(*gameRooms[nowroomID].player1Socket, asio::buffer(msg));
                asio::write(*gameRooms[nowroomID].player2Socket, asio::buffer(msg));
                msg = "turn#" + parts[1] + "#" + to_string(gameRooms[nowroomID].player1ID);
                asio::write(*gameRooms[nowroomID].player1Socket, asio::buffer(msg));
                asio::write(*gameRooms[nowroomID].player2Socket, asio::buffer(msg));
                gameRooms[nowroomID].gamecount = 1;
            }
        }
        else if (parts[0] == "num") {
            nowroomID = stoi(parts[1]);
            string msg = receivedMessage + "#" + to_string(gameRooms[nowroomID].gamecount);
            asio::write(*gameRooms[nowroomID].player1Socket, asio::buffer(msg));
            asio::write(*gameRooms[nowroomID].player2Socket, asio::buffer(msg));
        }
        else if (parts[0] == "notend") {
            nowroomID = stoi(parts[1]);
            int clientID = stoi(parts[2]);
            string msg = "turn#" + parts[1] + "#";
            bool isEven = (gameRooms[nowroomID].gamecount % 2 == 0);
            int targetID = isEven ? gameRooms[nowroomID].player1ID : gameRooms[nowroomID].player2ID;
            if (gameRooms[nowroomID].player1ID == clientID) gameRooms[nowroomID].notendcnt[0] = 1;
            if (gameRooms[nowroomID].player2ID == clientID) gameRooms[nowroomID].notendcnt[1] = 1;
            msg += to_string(targetID);
            asio::write(*clientSocket, asio::buffer(msg));
            if (gameRooms[nowroomID].notendcnt[0] && gameRooms[nowroomID].notendcnt[1]) {
                gameRooms[nowroomID].notendcnt[0] = 0;
                gameRooms[nowroomID].notendcnt[1] = 0;
                gameRooms[nowroomID].gamecount++;
            }
        }
        else if (parts[0] == "end") {
            nowroomID = stoi(parts[1]);
            int winnerID = stoi(parts[2]);
            int result = stoi(parts[3]);
            if (result == 1) {
                if (gameRooms[nowroomID].player1ID == winnerID) { 
                    updateWinLoss(gameRooms[nowroomID].player1info.nickname, true);
                    updateWinLoss(gameRooms[nowroomID].player2info.nickname, false);
                    gameRooms[nowroomID].player1Status = 9;
                }
                if (gameRooms[nowroomID].player2ID == winnerID) {
                    updateWinLoss(gameRooms[nowroomID].player1info.nickname, false);
                    updateWinLoss(gameRooms[nowroomID].player2info.nickname, true);
                    gameRooms[nowroomID].player2Status = 9; 
                }
            }
        }
        else if (parts[0] == "restart") {
            nowroomID = stoi(parts[1]);
            int clientID = stoi(parts[2]);
            int restart = stoi(parts[3]);
            auto& room = gameRooms[nowroomID];
            if(restart == 1 && gameRooms[nowroomID].player1ID == clientID){
                gameRooms[nowroomID].player1Status = 1;
                if (gameRooms[nowroomID].player2Status != 1) {
                    gameRooms[nowroomID].gamecount = 1;
                }
            }
            else if (restart == 0 && gameRooms[nowroomID].player1ID == clientID) {
                
                cout << clientID << "번 클라이언트 종료\n";
                {
                    gameRooms[nowroomID].player1Status = -1;
                    cout << "상태변경완료" << endl;
                }
                handleClientDisconnection(clientInfo.clientID);
                cout << "방 제거 함수 완료" << endl;
                if (clientSocket && clientSocket->is_open()) {
                    cout << "소켓 닫기 전 상태 OK" << endl;
                    clientSocket->close();
                }
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
                {
                    gameRooms[nowroomID].player2Status = -1;
                    cout << "상태변경완료" << endl;
                }
                handleClientDisconnection(clientInfo.clientID);
                cout << "방 제거 함수 완료" << endl; 
                if (clientSocket && clientSocket->is_open()) {
                    cout << "소켓 닫기 전 상태 OK" << endl;
                    clientSocket->close();
                }
                break;
            }
            if (gameRooms[nowroomID].player1Status == 1 && gameRooms[nowroomID].player2Status == 1) {
                boardsize = 0;
                room.gamecount = 1;
                string msg1 = "new#1";
                string msg2 = "new#-1";
                //두 클라이언트 모두 재시작 동의
                cout << nowroomID << "번 방 재시작\n";
                cout << "[RESTART] player1Status: " << room.player1Status
                    << ", player2Status: " << room.player2Status
                    << ", gamecount: " << room.gamecount << endl;

                if (gameRooms[nowroomID].gamecount == 1) {
                    asio::write(*room.player1Socket, asio::buffer(msg1));
                    cout << clientID << " " << msg1 << "\n";
                    cout << "클라이언트 대기 중: " << clientID << "\n";
                    asio::write(*room.player2Socket, asio::buffer(msg2));
                }
                else if (gameRooms[nowroomID].gamecount == 2) {
                    asio::write(*room.player2Socket, asio::buffer(msg1));
                    cout << clientID << " " << msg1 << "\n";
                    cout << "클라이언트 대기 중: " << clientID << "\n";
                    asio::write(*room.player1Socket, asio::buffer(msg2));
                }
            }
        }
        else if (parts[0] == "size") {
            nowroomID = stoi(parts[1]);
            boardsize = stoi(parts[2]);
            string msg = "start#" + to_string(nowroomID) + "#" + to_string(boardsize);
            asio::write(*gameRooms[nowroomID].player1Socket, asio::buffer(msg));
            asio::write(*gameRooms[nowroomID].player2Socket, asio::buffer(msg));
            gameRooms[nowroomID].gamecount = 1;
        }
    }
    lock_guard<mutex> lock(clientsMutex);
    clients.erase(clientInfo.clientID);
}



void assignClientToRoom(shared_ptr<tcp::socket> clientSocket, UserInfo clientInfo) {
    static int boardsize = 0;
    lock_guard<mutex> lock(gameRoomsMutex);
    if (!waitingQueue.empty()) {
        auto waitingClient = waitingQueue.front();
        waitingQueue.pop();

        GameRoom room;
        room.player1ID = waitingClient.first.clientID;
        room.player1Socket = waitingClient.second;
        room.player1info = waitingClient.first;
        room.player2ID = clientInfo.clientID;
        room.player2Socket = clientSocket;
        room.player2info = clientInfo;
        gameRooms[roomID] = room;

        int retry = 0;
        while (boardsize == 0 && retry++ < 100) this_thread::sleep_for(chrono::milliseconds(100));
        if (boardsize == 0) {
            cerr << "[ERROR] boardsize 못 받아 assignClientToRoom 실패\n";
            return;
        }

        Sleep(1500);
        string stmsg = "start#" + to_string(roomID++) + "#" + to_string(boardsize);
        cout << stmsg <<endl;
        asio::write(*room.player1Socket, asio::buffer(stmsg));
        asio::write(*room.player2Socket, asio::buffer(stmsg));
    }
    else {
        waitingQueue.push({ clientInfo, clientSocket });
        asio::write(*clientSocket, asio::buffer("new#1"));

        char buffer[BUFFER_SIZE];
        asio::error_code ec;
        size_t len = clientSocket->read_some(asio::buffer(buffer), ec);
        if (ec || len == 0) return;

        buffer[len] = '\0';
        string receivedMessage(buffer);
        istringstream ss(receivedMessage);
        string item;
        vector<string> parts;
        while (getline(ss, item, '#')) parts.push_back(item);
        if (parts.size() >= 3) boardsize = stoi(parts[2]);
    }
}


int main() {
    mongoc_init();
    try {
        asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), PORT));
        int clientIdx = 1;

        cout << "서버 실행 중... 포트 " << PORT << " 에서 대기 중\n";

        while (true) {
            shared_ptr<tcp::socket> socket = make_shared<tcp::socket>(io);
            asio::error_code ec;
            acceptor.accept(*socket, ec);

            if (ec) {
                cerr << "클라이언트 연결 실패: " << ec.message() << "\n";
                continue;
            }

            cout << "클라이언트 연결됨: ID = " << clientIdx << "\n";
            {
                lock_guard<mutex> lock(clientsMutex);
                clients[clientIdx] = socket;
            }

            string idMsg = to_string(clientIdx);
            asio::write(*socket, asio::buffer(idMsg));

            while (true) {
                char buffer[BUFFER_SIZE];
                asio::error_code err;
                size_t len = socket->read_some(asio::buffer(buffer), err);
                if (err || len == 0) {
                    cerr << "닉네임 수신 실패: " << err.message() << "\n";
                    continue;
                }
                buffer[len] = '\0';
                string msg(buffer);
                // 닉네임 수신 및 처리
                if (!ec && len > 0) {
                    buffer[len] = '\0';
                    string msg(buffer);
                    if (msg.rfind("nickname#", 0) == 0) {
                        string nickname = msg.substr(9);
                        pair<bool, pair<int, int>> nicknameinfo = loginOrRegisterNickname(socket, nickname);
                        if (nicknameinfo.first) {
                            UserInfo userinfo;
                            userinfo.wins = nicknameinfo.second.first;
                            userinfo.losses = nicknameinfo.second.second;
                            userinfo.nickname = nickname;
                            userinfo.clientID = clientIdx;
                            cout << userinfo.nickname << " 대기열 추가 준비\n";
                            assignClientToRoom(socket, userinfo);
                            cout << userinfo.nickname <<" 대기열 추가 완료\n";
                            thread clientThread(handleClient, socket, userinfo);
                            clientThread.detach();
                            clientIdx++;
                            break;
                        }
                        else {
                            cout << "로그인 또는 회원가입 실패\n";
                        }
                    }
                }
            }



        }
    }
    catch (const std::exception& e) {
        cerr << "서버 예외 발생: " << e.what() << "\n";
        mongoc_cleanup();
        return 1;
    }
    mongoc_cleanup();
    return 0;
}
