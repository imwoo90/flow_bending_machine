#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ManualSales : public MachineState {
private:
    ManualSales() {}
    virtual void initialize();
public:
    static ManualSales* getInstance();

    virtual MachineState* pressKey(const char key);
};
