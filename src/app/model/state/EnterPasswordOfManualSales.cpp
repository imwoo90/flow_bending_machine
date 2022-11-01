#include "EnterPasswordOfManualSales.h"

#include "SystemSetting.h"
#include "ManualSales.h"

bool EnterPasswordOfManualSales::isMatched(int password) {
    return _database->getPasswordOfManualSales() == password;
}

MachineState* EnterPasswordOfManualSales::decide() {
    if (_isChangePasswords) {
        int password = std::stoi(_data["param_0"]);
        _database->setPasswordOfManualSales(password);
        return this;
    }
    return ManualSales::getInstance();
}

MachineState* EnterPasswordOfManualSales::cancel() {
    if (_isChangePasswords) {
        return PasswordChange::getInstance();
    }
    return SystemSetting::getInstance();;
}

void EnterPasswordOfManualSales::initialize() {
    // init data
    _data.clear();
    _data["state"] = "EnterPasswordOfManualSales";
    _data["param_0"] = "000000";
}

EnterPasswordOfManualSales* EnterPasswordOfManualSales::getInstance(bool changePassword) {
    static EnterPasswordOfManualSales singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}
