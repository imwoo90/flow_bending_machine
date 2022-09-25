#pragma once

#include <MachineState.h>
#include <MachineData.h>

class SelectTest : public MachineState {
private:
    SelectTest() {}
    virtual void initialize();
public:
    static SelectTest* getInstance();

    virtual MachineState* pressKey(const char key);
};
