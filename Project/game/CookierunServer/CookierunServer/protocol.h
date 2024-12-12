#define SERVERPORT 9000
#define BUFSIZE    512

enum PTYPE
{
    ENTER,
    EXIT,
    JUMP
};

#pragma once
#pragma pack(push, 1)

struct SC_Packet
{
    unsigned char size;
    int id;
    PTYPE type;
};

struct CS_Packet
{
    unsigned char size;
    int id;
    PTYPE type;
};


#pragma pack(pop)