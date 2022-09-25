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
    char buf[32];
    // init data
    _data.clear();
    _data["state"] = "PasswordChange";
    _data["param_0"] = itoa(_database->getPasswordOfSystemManagement(), buf, 10);
    _data["param_1"] = itoa(_database->getPasswordOfSystemSetting(), buf, 10);
    _data["param_2"] = itoa(_database->getPasswordOfVendingMachineModeSetting(), buf, 10);
    _data["param_3"] = itoa(_database->getPasswordOfMainManagement(), buf, 10);
    _data["param_4"] = itoa(_database->getPasswordOfAdditionalStock(), buf, 10);
    _data["param_5"] = itoa(_database->getPasswordOfManualSales(), buf, 10);
    _data["param_6"] = itoa(_database->getPasswordOfPasswordChange(), buf, 10);
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
        next = EnterPasswordOfSystemManagement::getInstance(changePasword);
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
