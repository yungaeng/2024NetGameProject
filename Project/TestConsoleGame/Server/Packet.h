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
    char character;
    int x;
    int y;
    int old_x;
    int old_y;
};

#pragma pack(pop)