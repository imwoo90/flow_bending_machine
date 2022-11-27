#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <FreeRTOS.h>
#include <timers.h>

class ListOfAdditionalStock : public MachineState {
private:
    bool _isDisablePresskey;
    int _page;
    TimerHandle_t _timer;

    ListOfAdditionalStock();
    virtual void initialize();

    void loadListOfAdditionalStock();
public:
    static ListOfAdditionalStock* getInstance();

    virtual MachineState* timeout(const int signal);
    virtual MachineState* ListOfAdditionalStock::releaseKey(const char key);
    virtual MachineState* pressKey(const char key);
};
