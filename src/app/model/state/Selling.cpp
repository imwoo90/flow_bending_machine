#include "Selling.h"
#include "EnterProductNumber.h"

void Selling::initialize() {
    // init data
    data.clear();
    data["state"] = "Selling";
}

Selling* Selling::getInstance() {
    static Selling singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* Selling::pressKey(const char key) {
    return EnterProductNumber::getInstance();
}