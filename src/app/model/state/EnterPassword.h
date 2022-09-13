#pragma once

#include <MachineState.h>
#include <MachineData.h>

class EnterPassword : public MachineState {
private:
    virtual bool isMatched(int password) = 0;
    virtual MachineState* decide() = 0;
    virtual MachineState* cancel() = 0;
public:
    bool _isChangePasswords;
    virtual MachineState* pressKey(const char key);
};

