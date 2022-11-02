#pragma once

#include "EnterPassword.h"


class EnterPasswordOfAdditionalStock : public EnterPassword {
private:
    EnterPasswordOfAdditionalStock() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfAdditionalStock* getInstance(bool changePassword = false);
};
