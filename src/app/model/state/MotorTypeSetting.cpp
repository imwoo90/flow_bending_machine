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
            int start = std::stoi(_data["param_0"]);
            if (start <= 0) {
                break;
            } else if(_database->getNumberOfRelays() < start) {
                break;
            }
            _selection = 1;
        } else if ( _selection == 1) {
            int start = std::stoi(_data["param_0"]);
            int end = std::stoi(_data["param_1"]);
            if (end < start) {
                break;
            } else if(_database->getNumberOfRelays() < end) {
                break;
            }
            _selection = 2;
        } else if ( _selection == 2) {
            int start = std::stoi(_data["param_0"]);
            int end = std::stoi(_data["param_1"]);
            int motorType = std::stoi(_data["param_2"]);
            if (!(0 < motorType && motorType <= 2)) {
                break;
            }

            // Get start and end channel
            int ch_start = 0;
            int ch_end = 0;
            for(int relayIdx = 0; relayIdx < end; relayIdx++) {
                int n = _database->getNumberOfChannels(relayIdx);
                ch_end += n;
                if (relayIdx < (start-1)) {
                    ch_start += n;
                }
            }

            //set channel in selected relay
            for(int ch = ch_start; ch < ch_end; ch++) {
                _database->setMotorType(ch, motorType);
            }
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
