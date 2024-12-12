#pragma once
class Networking
{
public:
	SOCKET client_socket;
	bool is_connected = false;

	int client_id;
	int other_state = 3;
public:
	int Init();
	void Run();
	void Exit();

	void sendEnter();
	void sendExit();
	void sendJump();

	int getother();
	void setother(int v);
	CObject* CreatePlayer();
};

