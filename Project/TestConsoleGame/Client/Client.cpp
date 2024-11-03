#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "..\..\TestConsoleGame\Server\protocol.h"
#include <mutex>
using namespace std;

mutex my_lock;

#define SERVERPORT 9000
#define BUFSIZE    512

const int MAP_SIZE = 12;
const char EMPTY = '.';
const char CHARACTER = '@';
char map[MAP_SIZE][MAP_SIZE];

int characterX, characterY;
int oldX, oldY;

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

// 캐릭터를 무작위 위치에 배치
void placeCharacter(char map[MAP_SIZE][MAP_SIZE]) {
    srand(static_cast<unsigned int>(time(0)));
    characterX = rand() % MAP_SIZE;
    characterY = rand() % MAP_SIZE;
    map[characterY][characterX] = CHARACTER;
}

// 맵을 처음 한 번만 그리는 함수
void printMap(char map[MAP_SIZE][MAP_SIZE]) {
    setCursorPosition(0, 0);  // 화면의 맨 위로 커서 이동
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            cout << map[i][j] << ' ';
        }
        cout << endl;
    }
}

// 캐릭터 위치 업데이트 함수
void updateCharacterPosition(char map[MAP_SIZE][MAP_SIZE], int oldX, int oldY) {
    setCursorPosition(oldX * 2, oldY);
    cout << EMPTY;  // 이전 캐릭터 위치를 지움

    setCursorPosition(characterX * 2, characterY);
    cout << CHARACTER;  // 새로운 캐릭터 위치 출력
}

// 캐릭터 이동 함수
void moveCharacter(char map[MAP_SIZE][MAP_SIZE], char direction, SOCKET sock) {

    // 키 입력에 따른 캐릭터 이동
    switch (direction) {
    case 'w': if (characterY > 0) characterY--; break;
    case 'a': if (characterX > 0) characterX--; break;
    case 's': if (characterY < MAP_SIZE - 1) characterY++; break;
    case 'd': if (characterX < MAP_SIZE - 1) characterX++; break;
    default: return;
    }

    Packet packet;
    packet.size = sizeof(Packet);
    packet.type = 1; // 위치 업데이트 타입
    packet.x = characterX;
    packet.y = characterY;

    send(sock, (char*)&packet, sizeof(packet), 0); // 서버에 위치 전송
}

DWORD WINAPI ReceiveThread(LPVOID arg) {
    SOCKET sock = *(SOCKET*)arg;
    Packet packet;

    while (true) {
        int retval = recv(sock, (char*)&packet, sizeof(packet), 0);
        if (retval > 0) {
            cout << "수신 패킷 - 크기: " << packet.size
                << ", 타입: " << packet.type
                << ", x: " << packet.x
                << ", y: " << packet.y << endl;

            oldX = characterX; // 이전 위치 저장
            oldY = characterY;

            // 서버에서 받은 좌표로 캐릭터 위치 이동
            characterX = packet.x;
            characterY = packet.y;
            

            // 화면에 새로운 위치 출력
            updateCharacterPosition(map, oldX, oldY);
        }
        else if (retval == 0 || retval == SOCKET_ERROR) {
            cout << "서버 연결 종료" << endl;
            break;
        }
    }
    return 0;
}

int main() {
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    SOCKET client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == INVALID_SOCKET)
    {
        cout << "소켓 생성 실패" << endl;
        return 1;
    }

    // connect()
    struct sockaddr_in serveraddr;
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
    // Recv스레드 핸들 생성
    HANDLE hRecvThread = CreateThread(NULL, 0, ReceiveThread, (LPVOID)&client_sock, 0, NULL);
    if (hRecvThread == NULL) {
        closesocket(client_sock);
    }
    else {
        CloseHandle(hRecvThread);
    }

    // 초기 맵 출력 및 캐릭터 배치
    initializeMap(map);
    placeCharacter(map);
    printMap(map);

    while (true) {
        if (_kbhit()) {
            char input = _getch();
            if (input == 'q') break;
            moveCharacter(map, input, client_sock);
        }
    }

    closesocket(client_sock);
    WSACleanup();
    return 0;
}

