#include "SalesInfo.h"

#include "MainManagement.h"

void SalesInfo::initialize() {
    char buf[32];
    // init data
    _data.clear();
    _data["state"] = "SalesInfo";
    _data["param_0"] = itoa(_database->getNumberOfTotalSales(), buf, 10);
    _data["param_1"] = itoa(_database->getMoneyOfTotalSales(), buf, 10);
    _data["param_2"] = itoa(_database->getNumberOfManualSales(), buf, 10);
    _data["param_3"] = itoa(_database->getMoneyOfManualSales(), buf, 10);
}

SalesInfo* SalesInfo::getInstance() {
    static SalesInfo singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* SalesInfo::pressKey(const char key) {
    const bool changePasword = true;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = MainManagement::getInstance();
        break;
    case '#':
        //3초간 눌렀을때 초기화
        break;
    }
    return next;
}
