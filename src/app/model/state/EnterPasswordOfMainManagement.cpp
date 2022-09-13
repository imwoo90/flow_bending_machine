#include "EnterPasswordOfMainManagement.h"

#include "SystemSetting.h"
#include "MainManagement.h"

bool EnterPasswordOfMainManagement::isMatched(int password) {
    return _database->getPasswordOfMainManagement() == password;
}

MachineState* EnterPasswordOfMainManagement::decide() {
    if (_isChangePasswords) {
        int password = std::stoi(_data["param_0"]);
        _database->setPasswordOfMainManagement(password);
        return this;
    }
    return MainManagement::getInstance();
}

MachineState* EnterPasswordOfMainManagement::cancel() {
    if (_isChangePasswords) {
        return PasswordChange::getInstance();
    }
    return SystemSetting::getInstance();
}

void EnterPasswordOfMainManagement::initialize() {
    // init data
    _data.clear();
    _data["state"] = "EnterPasswordOfMainManagement";
    _data["param_0"] = "000000";
}

EnterPasswordOfMainManagement* EnterPasswordOfMainManagement::getInstance(bool changePassword) {
    static EnterPasswordOfMainManagement singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}
