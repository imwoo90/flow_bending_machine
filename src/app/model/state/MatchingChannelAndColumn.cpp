#include "MatchingChannelAndColumn.h"

#include "VendingMachineModeSetting.h"

void MatchingChannelAndColumn::initialize() {
    // init data
    _data.clear();
    _data["state"] = "MatchingChannelAndColumn";
    _data["param_0"] = "000"; // 001 column
    _data["param_1"] = "000";
    _selection = 0;
}

MatchingChannelAndColumn* MatchingChannelAndColumn::getInstance() {
    static MatchingChannelAndColumn singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* MatchingChannelAndColumn::pressKey(const char key) {
    int column, channel;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = VendingMachineModeSetting::getInstance();
        break;
    case '#':
        if (_selection == 0) {
            _selection = 1;
        } else {
            _selection = 0;
            int column = std::stoi(_data["param_0"])-1;
            int channel = std::stoi(_data["param_1"])-1;
            int maxCol = _database->getNumberOfColumns();

            //out of range
            if( !(0 <= column && column < maxCol) || !(0 <= channel && channel < maxCol))
                break;

            _database->setChannel(column, channel);
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
