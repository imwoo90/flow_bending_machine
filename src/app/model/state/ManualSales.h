#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ManualSales : public MachineState {
private:
    int _column, _is_running;

    ManualSales() {}
    virtual void initialize();
public:
    static ManualSales* getInstance();

    virtual MachineState* systemMessage(const SystemMessage mgs);
    virtual MachineState* pressKey(const char key);
};
