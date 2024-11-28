#pragma comment(lib, "ws2_32")

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\..\TestConsoleGame\Server\Packet.h"
#include <windows.h>
#include <string>
#include <thread>
using namespace std;
bool recving = true;
char client_map[MAP_SIZE][MAP_SIZE];

DWORD WINAPI recv_thread(LPVOID arg)
{
    SOCKET client_socket = (SOCKET)arg;
    char recvbuf[BUFSIZE];

    while (recving) {
        int ret = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
        if (ret == SOCKET_ERROR) {
            int error_code = WSAGetLastError();
            cout << "recv 실패. 에러 코드: " << error_code << endl;
            break;
        }
        else if (ret == 0) {
            cout << "[TCP 서버] 연결 종료" << endl;
            recving = false;
            closesocket(client_socket);
            return 0;
            break;
        }
        else if (ret >= sizeof(TestPacket)) {
            TestPacket* pp = reinterpret_cast<TestPacket*>(recvbuf);
            memcpy_s(client_map, sizeof(client_map), pp->map, sizeof(pp->map));

            // 맵 출력
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    cout << client_map[y][x] << " ";
                }
                cout << endl;
            }
        }
        else {
            cout << "수신 데이터가 예상된 패킷 크기와 다릅니다." << endl;
        }

        Sleep(16);
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
        return -1;

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    bool is_connected = true;
    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR)
    {
        cout << "연결이 끊겼습니다." << endl;
        is_connected = false;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // 이후 서버와 연결 됨
    cout << "서버를 찾았습니다!!" << endl;
    int addrlen = sizeof(server_addr);
    getpeername(client_socket, (struct sockaddr*)&server_addr, &addrlen);
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, addr, sizeof(addr));
    cout << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << addr << ", 포트 번호 = " << ntohs(server_addr.sin_port) << endl;

    // 수신 루프
    if (client_socket != INVALID_SOCKET) {
        HANDLE hWorkerThread = CreateThread(NULL, 0, recv_thread, (LPVOID)client_socket, 0, NULL);
        if (hWorkerThread == NULL) {
            closesocket(client_socket);
        }
        else {
            CloseHandle(hWorkerThread);
        }
    }

    // 송신 루프
    while (is_connected)
    {
        string message;
        getline(cin, message);

        if (message == "/quit")
        {
            is_connected = false;
            break;
        }
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
