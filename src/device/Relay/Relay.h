#ifndef RELAY_H
#define RELAY_H

#include <ModbusMaster.h>

class Relay {
private:
    ModbusMaster node;
public:
    Relay(uint8_t id, Stream &serial, void(*preTx)(), void(*postTx)());
    uint8_t open(uint16_t addr);
    uint8_t close(uint16_t addr);
};

#endif