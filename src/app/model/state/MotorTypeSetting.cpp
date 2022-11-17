#include "MotorTypeSetting.h"

#include "VendingMachineModeSetting.h"

void MotorTypeSetting::initialize() {
    char price[32];
    // init data
    _data.clear();
    _data["state"] = "MotorTypeSetting";
    _data["param_0"] = "00";
    _data["param_1"] = "00";
    _data["param_2"] = "00";
    _selection = 0;
}

MotorTypeSetting* MotorTypeSetting::getInstance() {
    static MotorTypeSetting singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* MotorTypeSetting::pressKey(const char key) {
    char price[32];
    int column, numOfGoods;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = VendingMachineModeSetting::getInstance();
        break;
    case '#':
        if ( _selection == 0) {
            _selection = 1;
        } else if ( _selection == 1) {
            _selection = 2;
        } else if ( _selection == 2) {
            _selection = 0;
            int start = std::stoi(_data["param_0"]);
            int end = std::stoi(_data["param_1"]);
            int motorType = std::stoi(_data["param_2"]);
            if ( start <= 0)
                break;
            else if (end < start)
                break;
            else if (_database->getNumberOfColumns() < end)
                break;

            for(int i = start; i <= end; i++) {
                // relay index is i-1 (started zero index)
                _database->setMotorType(i-1, motorType);
            }
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
