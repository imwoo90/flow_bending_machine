#include "VendingMachineModeSetting.h"

#include "SystemSetting.h"

void VendingMachineModeSetting::initialize() {
    // init data
    _data.clear();
    _data["state"] = "VendingMachineModeSetting";
}

VendingMachineModeSetting* VendingMachineModeSetting::getInstance() {
    static VendingMachineModeSetting singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* VendingMachineModeSetting::pressKey(const char key) {
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
    case '5':
        // Not implemented yet
        break;
    case '6':
        // Not implemented yet
        break;
    case '7':
        // Not implemented yet
        break;
    }
    return next;
}
