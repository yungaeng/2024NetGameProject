#define SERVERPORT 9000
#define BUFSIZE    512

enum CSTYPE
{
    ENTER,
    EXIT,
    JUMP,
    CIDLE
};

enum SCTYPE
{
    JOIN,
    QUIT,
    MOVE,
    SIDLE
};

#pragma once
#pragma pack(push, 1)

struct SC_Packet
{
    unsigned char size;
    int id;
    // 1번 플레이어는 0번플레이어 위치를 모름
    float x, y;
    int coin;
    SCTYPE type;
};

struct CS_Packet
{
    unsigned char size;
    int id;
    float x, y;
    int coin;
    CSTYPE type;
};


#pragma pack(pop)