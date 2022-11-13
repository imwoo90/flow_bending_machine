#include "ManualSales.h"

#include "SystemSetting.h"

void ManualSales::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "ManualSales";
    _data["param_0"] = "000"; // 001 column
    _data["param_1"] = "000000";
}

ManualSales* ManualSales::getInstance() {
    static ManualSales singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ManualSales::pressKey(const char key) {
    static int column, is_running = false;
    MachineState* next = this;
    char buf[32];

    _data["LockerType"] = "";
    _data["LockerChannel"] = "";
    _data["keyEvent"] = "";

    switch ( key ) {
    case 'S': //Sale
        if (is_running == false)
            break;

        _database->setNumberOfManualSales(1 + _database->getNumberOfManualSales());
        _database->setMoneyOfManualSales(std::stoi(_data["param_1"]) + _database->getMoneyOfManualSales());
        _data["LockerType"] = itoa(_database->getMotorType(column), buf, 10);
        _data["LockerChannel"] = itoa(_database->getChannel(column), buf, 10);
        is_running = false;
        break;
    case '*':
        is_running = false;
        next = SystemSetting::getInstance();
        break;
    case '#':
        if (is_running == true)
            break;
        column = std::stoi(_data["param_0"]) - 1;
        if (0<= column && column < _database->getNumberOfColumns() ) {
            int price = _database->getPrice(column);
            if (price <= 0)
                break;

            _data["param_1"] = itoa(price, buf, 10);
            _data["keyEvent"] = "S";
            is_running = true;
        }
        break;
    default: {//1~9
        if (is_running == true)
            break;
        std::string &param_0 = _data["param_0"];
        rotate(param_0.begin(), param_0.begin()+1, param_0.end());
        param_0[param_0.length()-1] = key;
        break;}
    }
    return next;
}
