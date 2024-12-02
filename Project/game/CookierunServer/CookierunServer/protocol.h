#define SERVERPORT 9000
#define BUFSIZE    512

#pragma once

#pragma pack(push, 1)

struct SC_Packet
{
    unsigned char size;
    int id;
    float x;
    float y;
};

struct CS_Packet
{
    unsigned char size;
    int id;
    float x;
    float y;
};


#pragma pack(pop)