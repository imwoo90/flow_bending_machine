#pragma once

#include "EnterPassword.h"


class EnterPasswordOfSystemManagement : public EnterPassword {
private:
    EnterPasswordOfSystemManagement() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfSystemManagement* getInstance(bool changePassword = false);
};
