#include "Relay.h"

Relay::Relay(uint8_t id, Stream &serial, void(*preTx)(), void(*postTx)()) {
    node.begin(id, serial);
    node.preTransmission(preTx);
    node.postTransmission(postTx);
}

uint8_t Relay::open(uint16_t addr) {
    uint8_t ret = node.writeSingleRegister(addr, 0x0100);
    delay(50);
    return ret;
}

uint8_t Relay::close(uint16_t addr) {
    uint8_t ret = node.writeSingleRegister(addr, 0x0200);
    delay(50);
    return ret;
}