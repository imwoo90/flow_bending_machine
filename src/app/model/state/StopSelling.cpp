#include "StopSelling.h"

void StopSelling::initialize() {
    // init data
    _data.clear();
    _data["state"] = "StopSelling";
}

StopSelling* StopSelling::getInstance() {
    static StopSelling singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}
