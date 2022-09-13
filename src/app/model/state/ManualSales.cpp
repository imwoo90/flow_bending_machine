#include "ManualSales.h"

#include "SystemSetting.h"

void ManualSales::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "ManualSales";
    _data["param_0"] = "001"; // 001 column
    _data["param_1"] = itoa(_database->getPrice(1), price, 10); // 1 column price
}

ManualSales* ManualSales::getInstance() {
    static ManualSales singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ManualSales::pressKey(const char key) {
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = SystemSetting::getInstance();
        break;
    case '#':
        int column = std::stoi(_data["param_0"]);
        if ( column < _database->getNumberOfColumns() ) {
            // Not implemented yet
        }
        break;
    default: {//1~9
        std::string &param_0 = _data["param_0"];
        rotate(param_0.begin(), param_0.begin()+1, param_0.end());
        param_0[param_0.length()-1] = key;
        break;}
    }
    return next;
}
