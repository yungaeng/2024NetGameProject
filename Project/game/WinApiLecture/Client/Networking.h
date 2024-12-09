#pragma once
class Networking
{
public:
	SOCKET client_socket;
	bool is_connected = false;

	int client_id;
	float x, y;

public:
	int Init();
	void Run();
	void Exit();

	void sendData(float x, float y);
	CObject* returnPlayer();
};

