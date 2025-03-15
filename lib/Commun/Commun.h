#ifndef _COMMUN_LIB
#define _COMMUN_LIB
#include <Arduino.h>

typedef struct Message{
    uint8_t id;
    uint8_t len;
    uint8_t *data;
    uint8_t checksum;
    // uint8_t checksum;
}Message;
#define SIZE_FIFO 32 //maximum 150 du fait du type char

struct Position {
    float x;
    float y;
    float z;

    bool operator>(const Position& other) const {
        return x > other.x || y > other.y || z > other.z;
    }

    bool operator<(const Position& other) const {
        return x < other.x || y < other.y || z < other.z;
    }
};

#endif //_COMMUN_LIB