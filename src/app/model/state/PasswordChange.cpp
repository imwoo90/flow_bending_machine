#include "PasswordChange.h"

#include "VendingMachineModeSetting.h"

#include "EnterPasswordOfSystemManagement.h"
#include "EnterPasswordOfSystemSetting.h"
#include "EnterPasswordOfVendingMachineModeSetting.h"
#include "EnterPasswordOfMainManagement.h"
#include "EnterPasswordOfAdditionalStock.h"
#include "EnterPasswordOfManualSales.h"
#include "EnterPasswordOfPasswordChange.h"

void PasswordChange::initialize() {
    // init data
    _data.clear();
    _data["state"] = "PasswordChange";
}

PasswordChange* PasswordChange::getInstance() {
    static PasswordChange singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* PasswordChange::pressKey(const char key) {
    const bool changePasword = true;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = VendingMachineModeSetting::getInstance();
        break;
    case '1':
        next = EnterPasswordOfSystemManagement::getInstance();
        break;
    case '2':
        next = EnterPasswordOfSystemSetting::getInstance(changePasword);
        break;
    case '3':
        next = EnterPasswordOfVendingMachineModeSetting::getInstance(changePasword);
        break;
    case '4':
        next = EnterPasswordOfMainManagement::getInstance(changePasword);
        break;
    case '5':
        next = EnterPasswordOfAdditionalStock::getInstance(changePasword);
        break;
    case '6':
        next = EnterPasswordOfManualSales::getInstance(changePasword);
        break;
    case '7':
        next = EnterPasswordOfPasswordChange::getInstance(changePasword);
        break;
    }
    return next;
}
