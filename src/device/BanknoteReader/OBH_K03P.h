#pragma once

#include <FreeRTOS.h>
#include <queue.h>
#include <timers.h>

#include <Arduino.h>
#include "BanknoteReader.h"

class OBH_K03P : public BanknoteReader {
private:
    int _inhibitPin;
    int _vendPin;
    int _errorPin;

    OBH_K03P() {};
public:
    QueueHandle_t _q;
    TimerHandle_t _timer;
    int _cntPulse;

    static OBH_K03P* getInstance(int inhibitPin, int vendPin, int errorPin);
    virtual int initialized();
    virtual void enable();
    virtual void disable();
    virtual int getBillData();
};
