#pragma once

#include "EnterPassword.h"

class EnterPasswordOfManualSales : public EnterPassword {
private:
    EnterPasswordOfManualSales() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide();
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfManualSales* getInstance(bool changePassword = false);
};
