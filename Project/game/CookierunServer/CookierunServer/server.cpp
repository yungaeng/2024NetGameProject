#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include <vector>
#include <thread>
#include <mutex>

#include "protocol.h"
using namespace std;

struct Client
{
    SOCKET socket;
    int id;
    float x, y;
};
vector<Client> Clients; // ���ӵ� ��� Ŭ���̾�Ʈ�� ������ ����
mutex mylock;

// ���� ����
SOCKET listen_sock;
int Newid() { return Clients.size(); }

// ���� �ʱ�ȭ �Լ� (�Ϸ�)
int InitServer()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        cout << "���� ���� ����" << endl;
        return 1;
    }

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    int retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        cout << "Bind ����" << endl;
        return 1;
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) {
        cout << "Listen ����" << endl;
        return 1;
    }
    else {
        cout << "���� �غ� �Ϸ�" << ", PORT : " << SERVERPORT << endl;
        return 0;
    }
}

// Ŭ���̾�Ʈ�� �����ϸ� �����Ǵ� ���� ������
void WorkerThread(SOCKET client_sock, int id)
{
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    cout << "[TCP ����] Ŭ���̾�Ʈ ����, IP �ּ� : " << addr << ", ��Ʈ ��ȣ : " << ntohs(clientaddr.sin_port) << ", ID : " << id << endl;

    // ���� ������ ����
    int totalReceived = 0;
    char buffer[BUFSIZE];
    while (true)
    {
        int retval = recv(client_sock, buffer, BUFSIZE, 0);
        if (retval <= 0) break;
        CS_Packet* pp = reinterpret_cast<CS_Packet*>(buffer);
        cout << pp->x << " " << pp->y;
        totalReceived += retval;

        // �ٸ� Ŭ���̾�Ʈ���� ���� ������ ���� �� ���۷� ���
        mylock.lock();
        for (const auto& client : Clients) {
            if (client.socket != client_sock) {
                send(client.socket, buffer, retval, 0);
            }
        }
        mylock.unlock();
    }


    // Ŭ���̾�Ʈ ��Ͽ��� ���� ����
    mylock.lock();
    Clients.erase(std::remove_if(Clients.begin(), Clients.end(),
        [client_sock](const Client& client) { return client.socket == client_sock; }), Clients.end());
    mylock.unlock();
}

int main()
{
    // ���� �ʱ�ȭ
    if (InitServer())
        return -1;

    // ���� ���� ����
    while (true) {
        SOCKET clientSocket = accept(listen_sock, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        int id = Newid();
        mylock.lock();
        Clients.push_back({ clientSocket, id });
        mylock.unlock();

        // Ŭ��� id ������
        send(clientSocket, (char*)&id, sizeof(id), 0);

        thread(WorkerThread, clientSocket, id).detach();
    }

    closesocket(listen_sock);
    WSACleanup();
    return 0;
}