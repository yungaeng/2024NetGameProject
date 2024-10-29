#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <queue>

#define SERVERPORT 9000
#define BUFSIZE    512

// 서버 소켓
SOCKET listen_sock;
// RecvQueue
std::queue<char*> RecvQueue;

// 서버 초기화
int Init()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		std::cout << "소켓 생성 실패" << std::endl;
		return 1;
	}

	// 소켓 주소 설정
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		std::cout << "Bind 실패" << std::endl;
		return 1;
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		std::cout << "Lisen 실패" << std::endl;
		return 1;
	}
	else
	{
		std::cout << "서버 준비 됨" << std::endl;
		return 0;
	}
}

// Recv 스레드
DWORD WINAPI RecvThread(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen = sizeof(clientaddr);
	char buf[BUFSIZE];

	// 클라이언트 정보 얻기
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1)
	{
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			std::cout << "Recv 실패" << std::endl;
			break;
		}
		else if (retval == 0)
			break;

		RecvQueue.push(buf);
	}

	// 소켓 닫기
	closesocket(client_sock);
	std::cout << "[TCP 서버] 클라이언트 종료: IP 주소 : " << addr << " 포트 번호 : " << ntohs(clientaddr.sin_port) << std::endl;
	return 0;
}

int main()
{
	if (Init())
		return -1;

	while (1)
	{
		// 데이터 통신에 사용할 변수
		SOCKET client_sock;
		struct sockaddr_in6 clientaddr;  // IPv6용 구조체
		int addrlen;
		HANDLE hThread;

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock != INVALID_SOCKET)
		{
			// 접속한 클라이언트 정보 출력 (IPv6)
			char addr[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));  // IPv6 주소 출력 , 포트출력
			std::cout << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << addr << ", 포트 번호 = " << ntohs(clientaddr.sin6_port) << std::endl;

			// 해당 클라이언트에게 Recv스레드 생성
			hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)client_sock, 0, NULL);
			if (hThread == NULL) { closesocket(client_sock); }
			else { CloseHandle(hThread); }
		}

		// 받은 데이터가 있다면
		if (RecvQueue.size() > 0)
		{
			
		}
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}