#pragma once

#include "EnterPassword.h"

class EnterPasswordOfMainManagement : public EnterPassword {
private:
    EnterPasswordOfMainManagement() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfMainManagement* getInstance(bool changePassword = false);
};

