#include "SetGoods.h"

#include "MainManagement.h"

void SetGoods::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "SetGoods";
    _data["param_0"] = "000";
    _data["param_1"] = "000";
    _data["param_2"] = "00";
    _selection = 0;
}

SetGoods* SetGoods::getInstance() {
    static SetGoods singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* SetGoods::pressKey(const char key) {
    char price[32];
    int column, numOfGoods;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = MainManagement::getInstance();
        break;
    case '#':
        if ( _selection == 0) {
            int column = std::stoi(_data["param_0"]) - 1;
            if (0 <= column && column < _database->getNumberOfColumns()) {
                _selection = 1;
            }
        } else if ( _selection == 1) {
            int column = std::stoi(_data["param_0"]) - 1;
            int price = std::stoi(_data["param_1"]) * 1000;
            if (price <= 500000) {
                _database->setPrice(column, price);
                Serial.printf("%d \n\r", _database->getPrice(column));
                _selection = 2;
            }
        } else if ( _selection == 2) {
            int column = std::stoi(_data["param_0"]) - 1;
            int quantity = std::stoi(_data["param_2"]);
            _database->setQuantity(column, quantity);
            Serial.printf("%d \n\r", _database->getQuantity(column));
            _selection = 0;
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
