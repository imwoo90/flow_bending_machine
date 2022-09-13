#pragma once

#include <MachineState.h>
#include <MachineData.h>

class StopSelling : public MachineState {
private:
    StopSelling() {}
    virtual void initialize();
public:
    static StopSelling* getInstance();
};
