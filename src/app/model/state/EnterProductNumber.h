#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <FreeRTOS.h>
#include <timers.h>

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
