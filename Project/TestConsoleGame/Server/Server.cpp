#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <queue>
#include <mutex>
#include "Packet.h"

using namespace std;

SOCKET listen_sock;
std::vector<SOCKET> Clients; // 접속된 모든 클라이언트의 소켓을 관리

// 서버 초기화 함수 (성공)
int InitServer()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        return 1;
    }

    sockaddr_in serveraddr;
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

// 클라이언트가 접속하면 생성되는 전용 스레드
DWORD WINAPI WorkerThread(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    cout << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << addr << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << endl;

    char buf[BUFSIZE];
    while (true) {
        int retval = recv(client_sock, (char*)&buf, sizeof(buf), 0);
        if (retval == SOCKET_ERROR) {
            int error_code = WSAGetLastError();
            cout << "recv 실패. 에러 코드: " << error_code << endl;
            break;
        }
        else if (retval == 0) {
            // 연결 종료
            cout << "[TCP 서버] 클라이언트 연결 종료" << endl;
            break;
        }
        else if (retval > 0)
        {
            Packet packet;
            Packet* pb = reinterpret_cast<Packet*>(buf);

            packet.size = sizeof(Packet);
            packet.type = 0;
            packet.id = pb->id;
            packet.character = pb->character;
            packet.x = pb->x;
            packet.y = pb->y;

            for (auto& sock : Clients)
                send(sock, (char*)&packet, sizeof(packet), 0);

            cout << "수신 패킷 - 크기: " << pb->size << endl;
            cout << "수신 패킷 - 타입: " << pb->type << endl;
            cout << "수신 패킷 - id: " << pb->id << endl;
            cout << "수신 패킷 - key: " << pb->character << endl;
            cout << "수신 패킷 - x: " << pb->x << endl;
            cout << "수신 패킷 - y: " << pb->y << endl;
            break;
        }
    }
    closesocket(client_sock);
    return 0;
}

int main()
{
    // 서버 초기화
    if (InitServer())
        return -1;

    if (sizeof(Clients) > 1)
    {
        // 서버 메인 루프
        while (true) 
        {
            SOCKET client_sock;
            struct sockaddr_in clientaddr;
            int addrlen = sizeof(clientaddr);

            client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
            if (client_sock != INVALID_SOCKET) {
                Clients.emplace_back(client_sock);
                HANDLE hWorkerThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)client_sock, 0, NULL);
                if (hWorkerThread == NULL) {
                    closesocket(client_sock);
                }
                else {
                    CloseHandle(hWorkerThread);
                }
            }
        }
    }
    // 종료 시 모든 소켓 닫기
    for (SOCKET sock : Clients)
    {
        closesocket(sock);
    }

    WSACleanup();
    return 0;
}

