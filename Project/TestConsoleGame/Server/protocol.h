#pragma once

#pragma pack(push, 1)
struct Packet {
    int size;
    int type;
    int x;
    int y;
};
#pragma pack(pop)