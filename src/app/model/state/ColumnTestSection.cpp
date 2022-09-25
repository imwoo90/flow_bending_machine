#include "ColumnTestSection.h"

#include "SelectTest.h"

void ColumnTestSection::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ColumnTestSection";
    _data["param_0"] = "end";
    _data["param_1"] = "000";
    _data["param_2"] = "000";
    _selection = 1;
}

ColumnTestSection* ColumnTestSection::getInstance() {
    static ColumnTestSection singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ColumnTestSection::pressKey(const char key) {
    MachineState* next = this;
    _data["param_0"] = "end";
    switch ( key ) {
    case '#':
        if (_selection == 1) {
            _selection = 2;
        } else if (_selection == 2) {
            int column1 = std::stoi(_data["param_1"]) - 1;
            int column2 = std::stoi(_data["param_2"]) - 1;
            if (!(0 <= column1 && column1 < _database->getNumberOfColumns())) {
                break;
            } else if (!(0 <= column2 && column2 < _database->getNumberOfColumns())) {
                break;
            }
            _data["param_0"] = "start";
            _selection = 1;
        }
        break;
    case '*':
        next = SelectTest::getInstance();
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
