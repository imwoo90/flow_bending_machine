#pragma once

#include <MachineState.h>
#include <MachineData.h>

class SalesInfo : public MachineState {
private:
    SalesInfo() {}
    virtual void initialize();
public:
    static SalesInfo* getInstance();

    virtual MachineState* pressKey(const char key);
};
