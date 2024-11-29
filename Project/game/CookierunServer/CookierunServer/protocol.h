#define SERVERPORT 9000
#define BUFSIZE    512

#pragma once

#pragma pack(push, 1)

enum PTYPE
{
    LOGIN, MOVE
};

struct SC_TestPacket
{
    char size;
};

struct CS_TestPacket
{
    char size;
    int id;
    char key;
};


#pragma pack(pop)