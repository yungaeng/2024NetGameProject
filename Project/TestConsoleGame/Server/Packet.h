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
    int id;
    char key;
    int x;
    int y;
};
#pragma pack(pop)