#pragma once

#include "EnterPassword.h"

class EnterPasswordOfManualSales : public EnterPassword {
private:
    EnterPasswordOfManualSales() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfManualSales* getInstance(bool changePassword = false);
};
