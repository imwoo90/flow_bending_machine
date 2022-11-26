#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <FreeRTOS.h>
#include <timers.h>

class EnterProductNumberNoGoods : public MachineState {
private:
    TimerHandle_t _timer;
    EnterProductNumberNoGoods();
    virtual void initialize();
public:
    static EnterProductNumberNoGoods* getInstance();

    virtual MachineState* timeout(const int signal);
};
