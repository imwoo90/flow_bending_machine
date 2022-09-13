#include "MainManagement.h"

#include "SystemSetting.h"

void MainManagement::initialize() {
    // init data
    _data.clear();
    _data["state"] = "MainManagement";
}

MainManagement* MainManagement::getInstance() {
    static MainManagement singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* MainManagement::pressKey(const char key) {
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = SystemSetting::getInstance();
        break;
    case '1':
        // Not implemented yet
        break;
    case '2':
        // Not implemented yet
        break;
    case '3':
        // Not implemented yet
        break;
    case '4':
        // Not implemented yet
        break;
    }
    return next;
}
