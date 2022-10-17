#pragma once

#include <Arduino.h>

class Relay {
public:
    virtual uint8_t open(uint16_t addr) = 0;
    virtual uint8_t close(uint16_t addr) = 0;
};
