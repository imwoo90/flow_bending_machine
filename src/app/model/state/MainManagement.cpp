#include "MainManagement.h"

#include "SystemSetting.h"
#include "SalesInfo.h"
#include "SetGoods.h"
#include "SelectTest.h"

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
        next = SalesInfo::getInstance();
        break;
    case '2':
        next = SetGoods::getInstance();
        break;
    case '3':
        next = SelectTest::getInstance();
        break;
    case '4':
        // Not implemented yet
        break;
    }
    return next;
}
