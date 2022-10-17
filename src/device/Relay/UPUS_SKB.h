#pragma once

#include "Relay.h"

class UPUS_SKB : public Relay {
private:
    int _id;
    Stream* _serial;
    // preTransmission callback function; gets called before writing a Modbus message
    void (*_preTransmission)();
    // postTransmission callback function; gets called after a Modbus message has been sent
    void (*_postTransmission)();

    inline uint8_t checksum(uint8_t _buf[5]) {
        return (uint8_t)(_buf[0] ^ _buf[1] ^ _buf[2] ^ _buf[3]);
    }
public:
    UPUS_SKB(uint8_t id, Stream &serial, void(*preTx)(), void(*postTx)());
    virtual uint8_t open(uint16_t addr);
    virtual uint8_t close(uint16_t addr);
};