#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <FreeRTOS.h>
#include <timers.h>

class ListOfCurrentStock : public MachineState {
private:
    bool _isDisablePresskey;
    int _page;

    ListOfCurrentStock();
    virtual void initialize();

    void loadListOfCurrentStock();
public:
    static ListOfCurrentStock* getInstance();

    virtual MachineState* pressKey(const char key);
};
