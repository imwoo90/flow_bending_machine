#include "EnterPasswordOfAdditionalStock.h"
#include "SystemSetting.h"

bool EnterPasswordOfAdditionalStock::isMatched(int password) {
    return _database->getPasswordOfAdditionalStock() == password;
}

MachineState* EnterPasswordOfAdditionalStock::decide() {
    if (_isChangePasswords) {
        int password = std::stoi(_data["param_0"]);
        _database->setPasswordOfAdditionalStock(password);
        return this;
    }
    return this;
}

MachineState* EnterPasswordOfAdditionalStock::cancel() {
    if (_isChangePasswords) {
        return PasswordChange::getInstance();
    }
    return SystemSetting::getInstance();;
}

void EnterPasswordOfAdditionalStock::initialize() {
    // init data
    _data.clear();
    _data["state"] = "EnterPasswordOfAdditionalStock";
    _data["param_0"] = "000000";
}

EnterPasswordOfAdditionalStock* EnterPasswordOfAdditionalStock::getInstance(bool changePassword) {
    static EnterPasswordOfAdditionalStock singleton_instance;
    singleton_instance._isChangePasswords = changePassword;
    singleton_instance.initialize();
    return &singleton_instance;
}

