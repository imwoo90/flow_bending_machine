#include "EnterPasswordOfPasswordChange.h"
#include "VendingMachineModeSetting.h"
#include "PasswordChange.h"

bool EnterPasswordOfPasswordChange::isMatched(int password) {
    return _database->getPasswordOfPasswordChange() == password;
}

MachineState* EnterPasswordOfPasswordChange::decide(int password) {
    if (_isChangePasswords) {
        _database->setPasswordOfPasswordChange(password);
        return this;
    }
    return PasswordChange::getInstance();
}

MachineState* EnterPasswordOfPasswordChange::cancel() {
    if (_isChangePasswords) {
        return PasswordChange::getInstance();
    }
    return VendingMachineModeSetting::getInstance();;
}

void EnterPasswordOfPasswordChange::initialize() {
    // init data
    _data.clear();
    _data["state"] = "EnterPasswordOfPasswordChange";
    if (_isChangePasswords) {
        char buf[32];
        sprintf(buf, "%06d", _database->getPasswordOfPasswordChange());
        _data["param_0"] = buf;
    } else {
        _data["param_0"] = "000000";
    }
}

EnterPasswordOfPasswordChange* EnterPasswordOfPasswordChange::getInstance(bool changePassword) {
    static EnterPasswordOfPasswordChange singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}

