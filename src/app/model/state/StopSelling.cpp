#include "StopSelling.h"

void StopSelling::initialize() {
    // init data
    data.clear();
    data["state"] = "StopSelling";
}

StopSelling* StopSelling::getInstance() {
    static StopSelling singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* StopSelling::pressKey(const char key) {
    return this;
}

MachineState* StopSelling::releaseKey(const char key) {
    return this;
}