#pragma once

#include "EnterPassword.h"

class EnterPasswordOfSystemSetting : public EnterPassword {
private:
    EnterPasswordOfSystemSetting() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfSystemSetting* getInstance(bool changePassword = false);
};

