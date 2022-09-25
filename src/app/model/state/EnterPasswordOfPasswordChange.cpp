#include "EnterPasswordOfPasswordChange.h"
#include "VendingMachineModeSetting.h"
#include "PasswordChange.h"

bool EnterPasswordOfPasswordChange::isMatched(int password) {
    return _database->getPasswordOfPasswordChange() == password;
}

MachineState* EnterPasswordOfPasswordChange::decide() {
    if (_isChangePasswords) {
        int password = std::stoi(_data["param_0"]);
        _database->setPasswordOfPasswordChange(password);
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
    _data["param_0"] = "000000";
}

EnterPasswordOfPasswordChange* EnterPasswordOfPasswordChange::getInstance(bool changePassword) {
    static EnterPasswordOfPasswordChange singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}

