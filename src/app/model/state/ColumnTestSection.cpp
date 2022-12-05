#include "ColumnTestSection.h"

#include "WorkingTest.h"

void ColumnTestSection::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ColumnTestSection";
    _data["param_0"] = "000";
    _data["param_1"] = "000";
    _selection = 0;
    _data["selection"] = "param_0";
}

ColumnTestSection* ColumnTestSection::getInstance() {
    static ColumnTestSection singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ColumnTestSection::pressKey(const char key) {
    static int column_s, column_e, column, running_test = false;
    char buf[32];
    MachineState* next = this;

    _data["LockerType"] = "";
    _data["LockerChannel"] = "";
    _data["keyEvent"] = "";
    switch ( key ) {
    case 'L': // loop column_s ~ column_e
        if (running_test == false)
            break;

        _data["LockerType"] = itoa(_database->getMotorType(column), buf, 10);
        _data["LockerChannel"] = itoa(_database->getChannel(column), buf, 10);
        if (column < column_e) {
            _data["keyEvent"] = "L";
        } else {
            running_test = false;
        }
        column += 1;
        break;
    case '#':
        if (running_test == true)
            break;

        if (_selection == 0) {
            column_s = std::stoi(_data["param_0"]) - 1;
            if (!(0 <= column_s && column_s < _database->getNumberOfColumns())) {
                break;
            }
            _selection = 1;
            _data["selection"] = "param_1";
        } else if (_selection == 1) {
            column_e = std::stoi(_data["param_1"]) - 1;
            if (!(0 <= column_e && column_e < _database->getNumberOfColumns())) {
                break;
            }
            _selection = 0;
            _data["selection"] = "param_0";
            _data["keyEvent"] = "L";
            column = column_s;
            running_test = true;
        }
        break;
    case '*':
        running_test = false;
        next = WorkingTest::getInstance();
        break;
    default: {//1~9
        if (running_test == true)
            break;

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
