#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "..\..\TestConsoleGame\Server\protocol.h"
#include <vector>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

const int MAP_SIZE = 12;
const char EMPTY = '.';
const char CHARACTER = '@';
const char OTHERCTER = '0';
char map[MAP_SIZE][MAP_SIZE];

struct character {
    char charactertype;
    int characterX, characterY;
    int oldX, oldY;
};

vector<character> characters;

// 커서 위치 설정 함수
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}

// 맵을 초기화
void initializeMap(char map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            map[i][j] = EMPTY;
        }
    }
}

// 캐릭터를 무작위 위치에 배치하고 벡터에 추가
void placeCharacter(char map[MAP_SIZE][MAP_SIZE], char type) {
    character newChar;
    newChar.charactertype = type;

    // 무작위 위치를 찾을 때까지 반복
    do {
        newChar.characterX = rand() % MAP_SIZE;
        newChar.characterY = rand() % MAP_SIZE;
    } while (map[newChar.characterY][newChar.characterX] != EMPTY);

    newChar.oldX = newChar.characterX;
    newChar.oldY = newChar.characterY;
    characters.push_back(newChar);

    map[newChar.characterY][newChar.characterX] = newChar.charactertype;
}

// 맵을 처음 한 번만 그리는 함수
void printMap(char map[MAP_SIZE][MAP_SIZE]) {
    setCursorPosition(0, 0);

    // 맵 초기화
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            map[i][j] = EMPTY;
        }
    }

    // 각 캐릭터의 위치를 맵에 표시
    for (const auto& ch : characters) {
        map[ch.characterY][ch.characterX] = ch.charactertype;
    }

    // 맵 출력
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            cout << map[i][j] << ' ';
        }
        cout << endl;
    }
}

// 캐릭터 위치 업데이트 함수
void updateCharacterPosition(character& ch) {
    setCursorPosition(ch.oldX * 2, ch.oldY);
    cout << EMPTY;

    setCursorPosition(ch.characterX * 2, ch.characterY);
    cout << ch.charactertype;

    ch.oldX = ch.characterX;
    ch.oldY = ch.characterY;
}

// 캐릭터 이동 함수
void moveCharacter(character& ch, char direction, SOCKET sock) {
    ch.oldX = ch.characterX;
    ch.oldY = ch.characterY;

    // 키 입력에 따른 캐릭터 이동
    switch (direction) {
    case 'w': if (ch.characterY > 0) ch.characterY--; break;
    case 'a': if (ch.characterX > 0) ch.characterX--; break;
    case 's': if (ch.characterY < MAP_SIZE - 1) ch.characterY++; break;
    case 'd': if (ch.characterX < MAP_SIZE - 1) ch.characterX++; break;
    default: return;
    }

    move_Packet packet;
    packet.size = sizeof(move_Packet);
    packet.type = 1; // 위치 업데이트 타입
    packet.x = ch.characterX;
    packet.y = ch.characterY;

    send(sock, (char*)&packet, sizeof(packet), 0); // 서버에 위치 전송

    updateCharacterPosition(ch);
}

// 수신 스레드 함수
DWORD WINAPI ReceiveThread(LPVOID arg) {
    SOCKET sock = *(SOCKET*)arg;
    char packet[BUFSIZE];
    while (true) {
        int retval = recv(sock, (char*)&packet, sizeof(packet), 0);
        if (retval > 0) {
            if (packet[1] == 1) {  // 움직임 업데이트
                // 기존 캐릭터 위치 업데이트 로직
                // ...
            }
            else if (packet[1] == 0) {  // 로그인 패킷 수신 시
                login_Packet* lp = reinterpret_cast<login_Packet*>(packet);

                character othercharacter;
                othercharacter.charactertype = OTHERCTER;
                othercharacter.characterX = lp->x;
                othercharacter.characterY = lp->y;
                othercharacter.oldX = lp->x;
                othercharacter.oldY = lp->y;

                characters.push_back(othercharacter);  // othercharacter 추가

                printMap(map);  // 화면에 새로운 캐릭터 포함해 다시 출력
            }
        }
        else if (retval == 0 || retval == SOCKET_ERROR) {
            cout << "서버 연결 종료" << endl;
            break;
        }
    }
    return 0;
}


int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    SOCKET client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        return 1;
    }

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    int retval = connect(client_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        cout << "Connect 실패, 서버와 연결할 수 없습니다" << endl;
        return 1;
    }

    cout << "서버를 찾았습니다" << endl;

    HANDLE hRecvThread = CreateThread(NULL, 0, ReceiveThread, (LPVOID)&client_sock, 0, NULL);
    if (hRecvThread == NULL) {
        closesocket(client_sock);
    }
    else {
        CloseHandle(hRecvThread);
    }

    initializeMap(map);
    placeCharacter(map, CHARACTER);  // 캐릭터 배치
    printMap(map);

    while (true) {
        if (_kbhit()) {
            char input = _getch();
            if (input == 'q') break;
            moveCharacter(characters[0], input, client_sock); // 첫 번째 캐릭터 이동
        }
    }

    closesocket(client_sock);
    WSACleanup();
    return 0;
}
