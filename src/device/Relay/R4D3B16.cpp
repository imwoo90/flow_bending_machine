#include "R4D3B16.h"

R4D3B16::R4D3B16(uint8_t id, Stream &serial, void(*preTx)(), void(*postTx)()) {
    node.begin(id, serial);
    node.preTransmission(preTx);
    node.postTransmission(postTx);
}

uint8_t R4D3B16::open(uint16_t addr) {
    uint8_t ret = node.writeSingleRegister(addr+1, 0x0100);
    delay(50);
    return ret;
}

uint8_t R4D3B16::close(uint16_t addr) {
    uint8_t ret = node.writeSingleRegister(addr+1, 0x0200);
    delay(50);
    return ret;
}