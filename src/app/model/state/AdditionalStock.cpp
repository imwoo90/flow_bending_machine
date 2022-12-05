#include "AdditionalStock.h"

#include "SystemSetting.h"

void AdditionalStock::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "AdditionalStock";
    _data["param_0"] = "000"; // 001 column
    _data["param_1"] = "000000";
    _data["param_2"] = "00"; // number of goods
    _selection = 0;
    _data["selection"] = "param_0";
}

AdditionalStock* AdditionalStock::getInstance() {
    static AdditionalStock singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* AdditionalStock::pressKey(const char key) {
    char buf[32];
    int column, addGoods;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = SystemSetting::getInstance();
        break;
    case '#':
        if (_selection == 0) {
            column = std::stoi(_data["param_0"])-1;
            if (0 <= column && column < _database->getNumberOfColumns()) {
                int price = _database->getPrice(column);
                if (price <= 0)
                    break;

                _data["param_1"] = itoa(price, buf, 10);
                _selection = 2;
                _data["selection"] = "param_2";
            }
        } else if (_selection == 2) {
            column = std::stoi(_data["param_0"])-1;
            addGoods = std::stoi(_data["param_2"]);
            addGoods += _database->getAdditional(column);
            _database->setAdditional(column, addGoods);
            _database->setQuantity(column, addGoods+_database->getQuantity(column));
            _selection = 0;
            _data["selection"] = "param_0";
        }
        break;
    default: {//1~9
        char param[] = "param_0";
        param[6] = '0' + _selection;
        std::string &param_0 = _data[param];
        rotate(param_0.begin(), param_0.begin()+1, param_0.end());
        param_0[param_0.length()-1] = key;
        _data["selection"] = param;
        break;}
    }
    return next;
}
