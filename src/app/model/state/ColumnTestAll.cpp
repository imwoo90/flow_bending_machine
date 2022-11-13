#include "ColumnTestAll.h"

#include "WorkingTest.h"

void ColumnTestAll::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ColumnTestAll";
    _data["param_0"] = "000";
}

ColumnTestAll* ColumnTestAll::getInstance() {
    static ColumnTestAll singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ColumnTestAll::pressKey(const char key) {
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

        column_s = std::stoi(_data["param_0"]) - 1;
        column_e = _database->getNumberOfColumns()-1;
        if (!(0 <= column_s && column_s <= column_e)) {
            break;
        }

        _data["keyEvent"] = "L";
        column = column_s;
        running_test = true;

        break;
    case '*':
        running_test = false;
        next = WorkingTest::getInstance();
        break;
    default: {//1~9
        if (running_test == true)
            break;

        std::string &param_1 = _data["param_0"];
        rotate(param_1.begin(), param_1.begin()+1, param_1.end());
        param_1[param_1.length()-1] = key;
        break;}
    }
    return next;
}
