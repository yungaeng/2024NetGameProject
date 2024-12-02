#pragma once
class Networking
{
public:
	SOCKET client_socket;
	bool is_connected = false;

public:
	int Init();
	void Run();
	void Exit();
};

