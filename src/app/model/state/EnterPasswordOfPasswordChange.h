#pragma once

#include "EnterPassword.h"


class EnterPasswordOfPasswordChange : public EnterPassword {
private:
    EnterPasswordOfPasswordChange() {}
    virtual bool isMatched(int password);
    virtual MachineState* decide(int password);
    virtual MachineState* cancel();
    virtual void initialize();
public:
    static EnterPasswordOfPasswordChange* getInstance(bool changePassword = false);
};
