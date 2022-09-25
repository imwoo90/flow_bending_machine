#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ColumnTestSection : public MachineState {
private:
    int _selection;

    ColumnTestSection() {}
    virtual void initialize();
public:
    static ColumnTestSection* getInstance();

    virtual MachineState* pressKey(const char key);
};
