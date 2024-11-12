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