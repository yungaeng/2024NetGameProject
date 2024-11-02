#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <queue>
#include <mutex>

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

SOCKET listen_sock;
queue<pair<int, int>> RecvQueue;
mutex queue_mutex; // 큐 접근 보호를 위한 뮤텍스

// 클라이언트 위치 정보를 출력하는 함수
void RecvHandler(int x, int y)
{
    cout << "클라이언트의 현재 위치: (" << x << ", " << y << ")" << endl;
}

// 서버 초기화 함수
int Init()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    int retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        cout << "Bind 실패" << endl;
        return 1;
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) {
        cout << "Listen 실패" << endl;
        return 1;
    }
    else {
        cout << "서버 준비 완료" << endl;
        return 0;
    }
}

// 클라이언트로부터 위치 정보를 수신하는 스레드
DWORD WINAPI RecvThread(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    bool IsRunnig = true;
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    cout << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << addr << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << endl;

    char x, y;
    while (IsRunnig) {
        // 정수형 위치 수신

        int retval = recv(client_sock, &x, sizeof(int), 0);
        if (retval <= 0) {
            cout << "[TCP 서버] 클라이언트 종료: IP 주소 : " << addr << " 포트 번호 : " << ntohs(clientaddr.sin_port) << endl;
            break;
        }

        retval = recv(client_sock, &y, sizeof(int), 0);
        if (retval <= 0) {
            cout << "[TCP 서버] 클라이언트 종료: IP 주소 : " << addr << " 포트 번호 : " << ntohs(clientaddr.sin_port) << endl;
            break;
        }

        else
        {
            IsRunnig = false;
        }

        // 큐에 위치 정보 추가
        queue_mutex.lock();
        RecvQueue.push(make_pair(x, y));
        queue_mutex.unlock();
    }

    closesocket(client_sock);
    return 0;
}

int main()
{
    if (Init())
        return -1;

    while (true) {
        SOCKET client_sock;
        struct sockaddr_in clientaddr;
        int addrlen = sizeof(clientaddr);

        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock != INVALID_SOCKET) {
            HANDLE hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
            if (hThread == NULL) {
                closesocket(client_sock);
            }
            else {
                CloseHandle(hThread);
            }
        }

        while (!RecvQueue.empty()) {
            // 큐에서 위치 정보를 가져오기 전에 뮤텍스를 잠금
            queue_mutex.lock();
            pair<int, int> position = RecvQueue.front();
            RecvQueue.pop();
            queue_mutex.unlock();

            RecvHandler(position.first, position.second);
        }
    }

    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
