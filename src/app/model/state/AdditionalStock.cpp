#include "AdditionalStock.h"

#include "SystemSetting.h"

void AdditionalStock::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "AdditionalStock";
    _data["param_0"] = "001"; // 001 column
    _data["param_1"] = itoa(_database->getPrice(1), price, 10); // 1 column price
    _data["param_2"] = "00"; // number of goods
    _selection = 0;
}

AdditionalStock* AdditionalStock::getInstance() {
    static AdditionalStock singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* AdditionalStock::pressKey(const char key) {
    char price[32];
    int column, numOfGoods;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = SystemSetting::getInstance();
        break;
    case '#':
        if (_selection == 1) {
            //save additional stock
            column = std::stoi(_data["param_0"]);
            numOfGoods = std::stoi(_data["param_2"]);
            numOfGoods += _database->getAdditional(column);
            _database->setAdditional(column, numOfGoods);
            _selection = 0;
        } else {
            column = std::stoi(_data["param_0"]);
            _data["param_1"] = itoa(_database->getPrice(column), price, 10);
            _selection = 1;
        }
        break;
    default: {//1~9
        char param[] = "param_0";
        param[6] = '0' + _selection;
        std::string &param_0 = _data[param];
        rotate(param_0.begin(), param_0.begin()+1, param_0.end());
        param_0[param_0.length()-1] = key;
        break;}
    }
    return next;
}
