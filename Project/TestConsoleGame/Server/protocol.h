#pragma once

#pragma pack(push, 1)
struct move_Packet {
    char size;
    char type;
    int x;
    int y;
};

struct login_Packet {
    char size;
    char type;
    int id;
    int x;
    int y;
};
#pragma pack(pop)