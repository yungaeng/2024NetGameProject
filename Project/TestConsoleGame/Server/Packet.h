#define SERVERPORT 9000
#define BUFSIZE    512
#define MAP_SIZE    10

#pragma once

#pragma pack(push, 1)

enum PTYPE
{
    LOGIN, MOVE
};

struct SC_TestPacket
{
    char size;
    char map[MAP_SIZE][MAP_SIZE];
};

struct CS_TestPacket
{
    char size;
    int id;
    char key;
};


#pragma pack(pop)