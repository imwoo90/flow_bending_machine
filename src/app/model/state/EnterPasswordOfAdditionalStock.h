#pragma once

#include "EnterPassword.h"


class EnterPasswordOfAdditionalStock : public EnterPassword {
private:
    EnterPasswordOfAdditionalStock() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide();
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfAdditionalStock* getInstance(bool changePassword = false);
};
