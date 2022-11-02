#include "EnterPasswordOfSystemManagement.h"
#include "SystemSetting.h"

bool EnterPasswordOfSystemManagement::isMatched(int password) {
    return _database->getPasswordOfSystemManagement() == password;
}

MachineState* EnterPasswordOfSystemManagement::decide(int password) {
    if (_isChangePasswords) {
        _database->setPasswordOfSystemManagement(password);
        return this;
    }
    return this;
}

MachineState* EnterPasswordOfSystemManagement::cancel() {
    if (_isChangePasswords) {
        return PasswordChange::getInstance();
    }
    return SystemSetting::getInstance();
}

void EnterPasswordOfSystemManagement::initialize() {
    // init data
    _data.clear();
    _data["state"] = "EnterPasswordOfSystemManagement";
    if (_isChangePasswords) {
        char buf[32];
        _data["param_0"] = itoa(_database->getPasswordOfSystemManagement(), buf, 10);
    } else {
        _data["param_0"] = "000";
    }
}

EnterPasswordOfSystemManagement* EnterPasswordOfSystemManagement::getInstance(bool changePassword) {
    static EnterPasswordOfSystemManagement singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}

