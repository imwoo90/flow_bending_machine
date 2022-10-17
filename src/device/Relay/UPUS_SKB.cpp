#include "UPUS_SKB.h"

UPUS_SKB::UPUS_SKB(uint8_t id, Stream &serial, void(*preTx)(), void(*postTx)()) {
    _id = id;
    _serial = &serial;
    _preTransmission = preTx;
    _postTransmission = postTx;
}

uint8_t UPUS_SKB::open(uint16_t addr) {
    uint8_t ret = 0;
    uint8_t buf[5] = {0x9a, (uint8_t)_id, (uint8_t)addr, 0x33};
    buf[4] = checksum(buf);
    Serial.printf("%02x %02x %02x %02x %02x\n\r", buf[0], buf[1], buf[2], buf[3], buf[4]);
    _preTransmission();
    _serial->write(buf, 5);
    _postTransmission();

    buf[4] = 0;
    _serial->readBytes(buf, 5);
    if (checksum(buf) != buf[4]) {
        Serial.println("UPUS_SKB open error");
        ret = -1;
    }
    return ret;
}

uint8_t UPUS_SKB::close(uint16_t addr) {
    uint8_t ret = 0;
    uint8_t buf[5] = {0x9b, (uint8_t)_id, (uint8_t)addr, 0x33};
    buf[4] = checksum(buf);
    _preTransmission();
    _serial->write(buf, 5);
    _postTransmission();

    buf[4] = 0;
    _serial->readBytes(buf, 5);
    if (checksum(buf) != buf[4]) {
        Serial.println("UPUS_SKB close error");
        ret = -1;
    }
    return ret;
}