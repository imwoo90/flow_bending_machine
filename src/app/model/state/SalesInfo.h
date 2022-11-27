#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <FreeRTOS.h>
#include <timers.h>

class SalesInfo : public MachineState {
private:
    bool _isDisablePresskey;
    int _page;
    TimerHandle_t _timer;

    SalesInfo();
    virtual void initialize();
public:
    static SalesInfo* getInstance();

    virtual MachineState* timeout(const int signal);
    virtual MachineState* pressKey(const char key);
    virtual MachineState* releaseKey(const char key);
};
