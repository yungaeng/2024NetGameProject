#pragma comment(lib, "ws2_32")

#include "Networking.h"
#include "global.h"
#include <ws2tcpip.h>
#include <thread>
#include "..\..\CookierunServer\CookierunServer\protocol.h"


using namespace std;

bool recving = true;

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
        else if (ret >= sizeof(SC_TestPacket)) {
            SC_TestPacket* pp = reinterpret_cast<SC_TestPacket*>(recvbuf);
           
        }
        else {
            cout << "수신 데이터가 예상된 패킷 크기와 다릅니다." << endl;
        }

    }
}

int Networking::Init()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
        return -1;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    is_connected = true;
    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR)
    {
        cerr << "연결이 끊겼습니다." << endl;
        is_connected = false;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // 이후 서버와 연결 됨
    cerr << "서버를 찾았습니다!!" << endl;
    int addrlen = sizeof(server_addr);
    getpeername(client_socket, (struct sockaddr*)&server_addr, &addrlen);
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, addr, sizeof(addr));
    cerr << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << addr << ", 포트 번호 = " << ntohs(server_addr.sin_port) << endl;
    return 0;
}

void Networking::Run()
{
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
        char message;
        cin >> message;

        if (message == 'q')
        {
            is_connected = false;
            break;
        }
        else
        {
            CS_TestPacket p;
            p.size = sizeof(p);
            p.id = 0;
            p.key = message;

            send(client_socket, (char*)&p, sizeof(p), 0);
            cerr << "client send data!!" << endl;
            break;
        }
    }
}

void Networking::Exit()
{
    closesocket(client_socket);
    WSACleanup();
}
