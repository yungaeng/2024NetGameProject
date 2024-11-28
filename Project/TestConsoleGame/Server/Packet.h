#define SERVERPORT 9000
#define BUFSIZE    512
#define MAP_SIZE    10

#pragma once
enum PTYPE
{
    LOGIN, MOVE
};

struct Packet {
    char size;
    char type;
    char data[510];
};

struct TestPacket
{
    char size;
    char type;
    char map[MAP_SIZE][MAP_SIZE];
};
#pragma pack(pop)