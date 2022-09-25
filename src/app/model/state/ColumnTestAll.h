#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ColumnTestAll : public MachineState {
private:
    ColumnTestAll() {}
    virtual void initialize();
public:
    static ColumnTestAll* getInstance();

    virtual MachineState* pressKey(const char key);
};
