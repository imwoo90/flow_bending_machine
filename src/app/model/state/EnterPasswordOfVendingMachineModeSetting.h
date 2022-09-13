#pragma once

#include "EnterPassword.h"

class EnterPasswordOfVendingMachineModeSetting : public EnterPassword {
private:
    EnterPasswordOfVendingMachineModeSetting() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide();
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfVendingMachineModeSetting* getInstance(bool changePassword = false);
};
