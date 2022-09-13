#pragma once

#include <FreeRTOS.h>
#include <timers.h>

#include <MachineState.h>
#include <MachineData.h>

class EnterProductNumber : public MachineState {
private:
    TimerHandle_t _timer;
    EnterProductNumber();
    virtual void initialize();
public:
    static EnterProductNumber* getInstance();
    virtual MachineState* pressKey(const char key);
    virtual MachineState* timeout(const int signal);
};
